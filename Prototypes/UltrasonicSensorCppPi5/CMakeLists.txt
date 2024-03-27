cmake_minimum_required(VERSION 3.27)
project(UltrasonicSensorCpp)

set(CMAKE_CXX_STANDARD 23)

# Find wiringPi library
find_library(WIRINGPI_LIBRARY NAMES wiringPi)

if(WIRINGPI_LIBRARY)
    message(STATUS "wiringPi library found: ${WIRINGPI_LIBRARY}")
    add_executable(UltrasonicSensorCpp main.cpp)
    target_link_libraries(UltrasonicSensorCpp ${WIRINGPI_LIBRARY})
else()
    message(FATAL_ERROR "wiringPi library not found >:(")
endif()
