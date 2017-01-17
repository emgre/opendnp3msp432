cmake_minimum_required(VERSION 2.8)

include(CMakeForceCompiler)

set(TI_INSTALL_DIR C:\\ti\\msp432_gcc CACHE FILEPATH "Path to TI MSP432 directory.")

set(DEVICE msp432p401r)
string(TOUPPER ${DEVICE} DEVICE_UPPER)

set(COMPILER_PATH ${TI_INSTALL_DIR}/arm_compiler)
set(MSP_INC_DIR ${TI_INSTALL_DIR}/arm/include)
set(CMSIS_INC_DIR ${MSP_INC_DIR}/CMSIS)
set(GCC_INC_DIR ${COMPILER_PATH}/arm-none-eabi/include)
set(LDS_FILE ${MSP_INC_DIR}/${DEVICE}.lds)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_FIND_ROOT_PATH ${COMPILER_PATH})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Find the compilers
find_program(GCC_C NAMES arm-none-eabi-gcc PATHS ${COMPILER_PATH}/bin NO_DEFAULT_PATH)
find_program(GCC_CXX NAMES arm-none-eabi-g++ PATHS ${COMPILER_PATH}/bin NO_DEFAULT_PATH)
if(NOT GCC_C OR NOT GCC_CXX)
    message(FATAL_ERROR "Compiler could not be found. Please set TI_INSTALL_DIR variable.")
endif()

CMAKE_FORCE_C_COMPILER(${GCC_C} GNU)
CMAKE_FORCE_CXX_COMPILER(${GCC_CXX} GNU)

# Add definitions and flags
add_definitions(-D__${DEVICE_UPPER}__ -DTARGET_IS_MSP432P4XX -Dgcc)

set(COMMON_FLAGS "-mcpu=cortex-m4 -march=armv7e-m -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mthumb -Wall -g -fmessage-length=0 -fno-exceptions -ffunction-sections -fdata-sections -fsingle-precision-constant")
set(CMAKE_C_FLAGS "${COMMON_FLAGS} -std=c99" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS "${COMMON_FLAGS} -fno-rtti -std=c++11" CACHE STRING "" FORCE)

set(CMAKE_EXE_LINKER_FLAGS "-T${LDS_FILE} -Wl,--gc-sections -lc -lgcc -lnosys" CACHE STRING "" FORCE)

# Add include directories
include_directories(${GCC_INC_DIR} ${CMSIS_INC_DIR} ${MSP_INC_DIR})