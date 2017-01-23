cmake_minimum_required(VERSION 2.8)

set(LIBNAME "OpenDNP3.out")

project(OpenDNP3 C CXX)
if(NOT CMAKE_CROSSCOMPILING)
    message(FATAL_ERROR "This project must be cross-compiled. Please set CMAKE_TOOLCHAIN_FILE to point to msp432-toolchain.cmake")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T${PROJECT_SOURCE_DIR}/msp432p401r.lds -specs nosys.specs -specs nano.specs")
set(CMAKE_BUILD_TYPE RelWithDebInfo)

add_subdirectory(opendnp3 EXCLUDE_FROM_ALL)
include_directories(opendnp3/cpp/libs/include)
include_directories(opendnp3/cpp/libs/src)

add_subdirectory(driverlib)
include_directories(driverlib)

set(SRC
    startup_msp432p401r_gcc.c
    system_msp432p401r.c
	main.cpp
    Executor.cpp
    Timer.cpp
    CommandHandler.cpp
    LinkRouter.cpp
)

add_executable(${LIBNAME} ${SRC})
target_link_libraries(${LIBNAME} opendnp3)
target_link_libraries(${LIBNAME} driverlib)