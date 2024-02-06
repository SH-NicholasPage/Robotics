cmake_minimum_required(VERSION 3.27)
project(UltrasonicSensor C)

set(CMAKE_C_STANDARD 23)

# Find pigpio library
find_library(PIGPIO_LIBRARY NAMES pigpio)

if(PIGPIO_LIBRARY)
    message(STATUS "pigpio library found: ${PIGPIO_LIBRARY}")
    add_executable(UltrasonicSensor main.c)
    target_link_libraries(UltrasonicSensor ${PIGPIO_LIBRARY})
else()
    message(FATAL_ERROR "pigpio library not found >:(")
endif()
