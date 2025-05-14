cmake_minimum_required(VERSION 3.12)

# Indlæs Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(picoCode)

# Initialiser Pico SDK
pico_sdk_init()

# Definer dit program
add_executable(picoMain picoMain.cpp Gripper.cpp)

# Mulighed for at bruge USB som seriel port
pico_enable_stdio_usb(picoMain 1)
pico_enable_stdio_uart(picoMain 0)

# Generer ekstra output-filer (uf2)
pico_add_extra_outputs(picoMain)

# Link standard Pico biblioteker
target_link_libraries(picoMain pico_stdlib hardware_adc)

