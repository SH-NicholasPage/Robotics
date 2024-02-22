cmake_minimum_required(VERSION 3.27)
project(SHADCDevice)

set(CMAKE_CXX_STANDARD 23)

# Find pigpio library
find_library(PIGPIO_LIBRARY NAMES pigpio)

if(PIGPIO_LIBRARY)
    message(STATUS "pigpio library found: ${PIGPIO_LIBRARY}")
    add_executable(SHADCDevice ADCDevice.cpp)
    target_link_libraries(SHADCDevice ${PIGPIO_LIBRARY})
else()
    message(FATAL_ERROR "pigpio library not found >:(")
endif()
