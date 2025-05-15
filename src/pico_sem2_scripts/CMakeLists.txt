cmake_minimum_required(VERSION 3.12)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(picoCode)

pico_sdk_init()

file(GLOB src_files src/*.cpp)


add_executable(picoMain mains/picoMain.cpp ${src_files})
add_executable(adcTestMain mains/adcTestMain.cpp ${src_files})

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/headers
)

pico_enable_stdio_usb(picoMain 1)
pico_enable_stdio_usb(adcTestMain 1)

pico_enable_stdio_uart(picoMain 0)
pico_enable_stdio_uart(adcTestMain 0)

pico_add_extra_outputs(picoMain)
pico_add_extra_outputs(adcTestMain)


target_link_libraries(picoMain pico_stdlib hardware_adc hardware_pwm)
target_link_libraries(adcTestMain pico_stdlib hardware_adc hardware_pwm)