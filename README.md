# OpenDNP3 on TI MSP432

This project tries to run [OpenDNP3](https://github.com/automatak/dnp3) on a [Texas Instruments MSP432P401R Launchpad](http://www.ti.com/tool/MSP-EXP432P401R) acting as an outstation. This merely a test, but the code is shared here in response to [this issue](https://github.com/automatak/dnp3/issues/189).

## How to build
CMake is used to generate the makefile this project. The project uses the GCC compiler, because I couldn't make it compile with TI's compiler. Since it involves cross-compiling, it is a bit more complicated to use. Follow these steps and you should be fine.

1. Download and install the latest [GCC Package for the MSP432](http://www.ti.com/tool/MSP430-GCC-OPENSOURCE) from TI's website. **Be sure to download the MSP432 package, not the MSP430.**
2. Download the latest [GNU ARM Embedded Toolchain](https://launchpad.net/gcc-arm-embedded/+download) and extract it in the `arm_compiler` directory inside the TI package directory.
3. Clone this repository using `git clone --recursive https://github.com/emgre/opendnp3msp432.git`. The recursive flag is mandatory in order to download OpenDNP3.
4. Create a `build` directory, `cd` into it, then run `cmake -G "MinGW Makefiles" -DCMAKE_TOOLCHAIN_FILE="../msp432-toolchain.cmake" -DTI_INSTALL_DIR="/path/to/TI/msp432_gcc/" ..` where you change the path to the TI folder for the appropriate one, and change the generator according to your platform (this code was only tested on a Windows machine, that's why the generator is a MingW makefile. On a Unix system, "Unix Makefiles" should be the right generator). If CMake complains about not finding the make program (if you haven't set it in your PATH variables), you can specify it by adding `-DCMAKE_MAKE_PROGRAM="path/to/make"`.
5. Run the `make` in the build directory to compile the executable.

## Debugging
To debug, follow the steps in the doc file available from TI. Briefly, it involes doing:

1. Installing the required drivers for the MSP432 Launchpad.
2. Connecting the MSP432 Launchpad to the computer.
3. Run `gdb_agent_console -f MSP432 xds110_msp432_jtag.dat` from `INSTALL_DIR/emulation/common/uscif`.
4. Run `arm-none-eabi-gdb "path/to/OpenDNP3.out"`.
5. In GDB, run `target remote :55000`, `load` (this takes a while...), `monitor reset`. Then, you're good to go. The standard GDB commands are available.

Note that the GDB agent made by TI does not specify the memory map. Therefore, placing breakpoints will fail because the code is executed from the Flash and GDB tries to insert software breakpoints. In order to force it using hardware breakpoints, you can do `mem 0 0 ro` to specify that all the memory is read-only. A proper memory map could help you modify variables at runtime.

## How to run
Simply build and flash the MSP432, then connect a master DNP3 on serial port COMx (mine is COM3) with a baudrate of 9600 and standard UART settings. I personnaly used the master demo of OpenDNP3 and modified 2-3 lines to get it running with the desired settings.