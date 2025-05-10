cmake_minimum_required(VERSION 3.8)
project(ur5_sem2_scripts)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Include directories
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)


# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(ur_client_library REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(moveit_ros_planning_interface REQUIRED)
find_package(moveit_msgs REQUIRED)
find_package(OpenCV REQUIRED)


# === Moveplanner Library ===
file(GLOB MOVE_SRCS CONFIGURE_DEPENDS src/moveplanner/*.cpp)
add_library(MoveplannerModule ${MOVE_SRCS})
target_include_directories(MoveplannerModule PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ur5_sem2_scripts
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ur5_sem2_scripts/moveplanner
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ur5_sem2_scripts/vision
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ur5_sem2_scripts/board
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
)
ament_target_dependencies(MoveplannerModule rclcpp geometry_msgs tf2_ros std_msgs moveit_ros_planning_interface)

# === Vision Library ===
file(GLOB VISION_SRCS CONFIGURE_DEPENDS src/vision/*.cpp)
add_library(VisionModule ${VISION_SRCS})
target_include_directories(VisionModule PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ur5_sem2_scripts/vision
    ${OpenCV_INCLUDE_DIRS}
)
target_link_libraries(VisionModule ${OpenCV_LIBS})
ament_target_dependencies(VisionModule rclcpp)

# === Board Library ===
file(GLOB BOARD_SRCS CONFIGURE_DEPENDS src/board/*.cpp)
add_library(BoardModule ${BOARD_SRCS})
target_include_directories(BoardModule PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ur5_sem2_scripts/board
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ur5_sem2_scripts/vision
    )
target_link_libraries(BoardModule VisionModule)
ament_target_dependencies(BoardModule rclcpp)

# === PicoComm Library ===
file(GLOB PICOCOMM_SRCS CONFIGURE_DEPENDS src/pico_comm/*.cpp)
add_library(PicoCommModule ${PICOCOMM_SRCS})
target_include_directories(PicoCommModule PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ur5_sem2_scripts/pico_comm
)
ament_target_dependencies(PicoCommModule rclcpp)



# Declare executables
add_executable(simple_move src/simple_move.cpp)
add_executable(svejse_frame src/svejse_frame.cpp)
add_executable(chess_frame src/chess_frame.cpp)
add_executable(square_move src/square_move.cpp)
add_executable(readJoints src/readJoints.cpp)
add_executable(robot_main src/robot_main.cpp src/BoardTransformer.cpp)
add_executable(vision_main src/vision_main.cpp)
add_executable(board_test_main src/TestBoard.cpp)

target_include_directories(robot_main PUBLIC include)
# Apply dependencies to all executables
ament_target_dependencies(simple_move rclcpp ur_client_library tf2_ros geometry_msgs std_msgs moveit_ros_planning_interface)
ament_target_dependencies(svejse_frame rclcpp ur_client_library tf2_ros geometry_msgs std_msgs moveit_ros_planning_interface)
ament_target_dependencies(chess_frame rclcpp ur_client_library tf2_ros geometry_msgs std_msgs moveit_ros_planning_interface)
ament_target_dependencies(square_move rclcpp ur_client_library tf2_ros geometry_msgs std_msgs moveit_ros_planning_interface)
ament_target_dependencies(readJoints rclcpp ur_client_library tf2_ros geometry_msgs std_msgs moveit_ros_planning_interface)
ament_target_dependencies(robot_main rclcpp ur_client_library tf2_ros geometry_msgs std_msgs moveit_ros_planning_interface)
target_link_libraries(robot_main MoveplannerModule VisionModule BoardModule)  # Link with the libraries
ament_target_dependencies(vision_main rclcpp)
target_link_libraries(vision_main VisionModule BoardModule)  # Link with the libraries
ament_target_dependencies(board_test_main rclcpp)
target_link_libraries(board_test_main VisionModule BoardModule)  # Link with the libraries

# Install executables
install(TARGETS simple_move svejse_frame chess_frame square_move readJoints robot_main vision_main board_test_main
  DESTINATION lib/${PROJECT_NAME})

# Install library (ChessMoves) properly
 install(TARGETS MoveplannerModule VisionModule BoardModule
   DESTINATION lib/${PROJECT_NAME})

# Install share files
install(DIRECTORY pictures DESTINATION share/${PROJECT_NAME})
   
# Install launch files
install(DIRECTORY launch DESTINATION share/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
