cmake_minimum_required(VERSION 3.27)
project(MotorTestCpp)

set(CMAKE_CXX_STANDARD 23)

# Find pigpio library
find_library(PIGPIO_LIBRARY NAMES pigpio)

# Add ADCDevice library flag
set(ADC_DEVICE_LIB -lADCDevice)

if(PIGPIO_LIBRARY)
    message(STATUS "pigpio library found: ${PIGPIO_LIBRARY}")
    add_executable(MotorTestCpp main.cpp)
    target_link_libraries(MotorTestCpp ${PIGPIO_LIBRARY})
else()
    message(FATAL_ERROR "pigpio library not found >:(")
endif()