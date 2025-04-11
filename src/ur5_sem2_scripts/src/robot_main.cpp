#include <memory>

#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.h>
#include <tf2/LinearMath/Quaternion.h>
#include <geometry_msgs/msg/pose.h>
#include <moveit_msgs/msg/joint_constraint.h>
#include <string>
#include <vector>
#include "ur5_sem2_scripts/ChessMoves.hpp"
#include "ur5_sem2_scripts/moveStruct.hpp"

int main(int argc, char * argv[])
{
  // Initialize ROS and create the Node
  rclcpp::init(argc, argv);
  auto const node = std::make_shared<rclcpp::Node>(
      "robot_main",
      rclcpp::NodeOptions().automatically_declare_parameters_from_overrides(true));

  // Create a ROS logger
  auto const logger = rclcpp::get_logger("robot_main");

  
  ChessMoves chessMoves(node);
  moveStruct move;
  move.piece = 'p';
  move.type = 'k';
  
  chessMoves.move_to_idle();
  // chessMoves.move(move, nullptr);
  // chessMoves.move(move, nullptr);
  // chessMoves.move(move, nullptr);
  // chessMoves.move(move, nullptr);

  // Shutdown ROS
  rclcpp::shutdown();
  return 0;
}




