cmake_minimum_required(VERSION 3.10)

project(Semesterprojekt_2)

find_package(OpenCV REQUIRED)

# Automatically find all .cpp files
file(GLOB SRC_FILES "*.cpp")

# Create the executable with all source files
add_executable(main ${SRC_FILES})

target_link_libraries(main ${OpenCV_LIBS})

include_directories(${OpenCV_INCLUDE_DIRS})
