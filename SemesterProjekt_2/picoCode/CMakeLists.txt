cmake_minimum_required(VERSION 3.12)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(picoCode)

pico_sdk_init()

add_executable(picoMain picoMain.cpp)

pico_enable_stdio_usb(picoMain 1)
pico_enable_stdio_uart(picoMain 0)

pico_add_extra_outputs(picoMain)

target_link_libraries(picoMain pico_stdlib hardware_adc hardware_pwm)