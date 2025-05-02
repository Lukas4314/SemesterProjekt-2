#include <memory>

#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.h>
#include <tf2/LinearMath/Quaternion.h>
#include <geometry_msgs/msg/pose.h>
#include <moveit_msgs/msg/joint_constraint.h>
#include <string>
#include <vector>
#include "ur5_sem2_scripts/moveplanner/ChessMoves.hpp"
#include "ur5_sem2_scripts/moveStruct.hpp"
#include "ur5_sem2_scripts/vision/AllInOneMain.h"
#include "ur5_sem2_scripts/BoardTransformer.hpp"

int main(int argc, char *argv[])
{
  // Initialize ROS and create the Node
  rclcpp::init(argc, argv);
  auto const node = std::make_shared<rclcpp::Node>(
      "robot_main",
      rclcpp::NodeOptions().automatically_declare_parameters_from_overrides(true));

  // Create a ROS logger
  auto const logger = rclcpp::get_logger("robot_main");
  int camera_index;
  node->get_parameter("camera_index", camera_index);  // Retrieve the parameter



  ChessMoves chessMoves(node);
  chessMoves.move_to_idle();

  
  AllInOneMain allInOneMain = AllInOneMain(camera_index);
  allInOneMain.getPieceMovedString(0);
  std::array<std::array<double, 4>, 4> TFcamchess = allInOneMain.getBoardCutter(0).getTFchess();
  std::array<std::array<double, 4>, 4> TFcamPlokker = allInOneMain.getBoardCutter(1).getTFchess();
  std::array<std::array<double, 4>, 4> TFPlokkerCam = BoardTransformer::getInverse(TFcamPlokker);
  std::array<std::array<double, 4>, 4> TFPlokkerChess = BoardTransformer::multiplyMatrices(TFcamPlokker, TFcamchess);
  std::array<std::array<double, 4>, 4> TF = BoardTransformer::multiplyMatrices(TFPlokkerChess,chessMoves.TFRed);


  moveStruct move;
  move.piece = 'p';
  move.type = 'm';
  move.start[0] = 0;
  move.start[1] = 0;
  move.end[0] = 7;
  move.end[1] = 7;
  move.captured = '-';
  move.color = 'w';



  double raw_TF[4][4];
  for (size_t i = 0; i < 4; ++i) {
    for (size_t j = 0; j < 4; ++j) {
      raw_TF[i][j] = TF[i][j];
    }
  }

  // output the transformation matrix


  RCLCPP_INFO(logger, "Transformation Matrix:");
  for (size_t i = 0; i < 4; ++i) {
    std::string row_str;
    for (size_t j = 0; j < 4; ++j) {
      row_str += std::to_string(raw_TF[i][j]) + " ";
    }
    RCLCPP_INFO(logger, "%s", row_str.c_str());
  }

  // chessMoves.move(move, raw_TF);

  // Shutdown ROS
  rclcpp::shutdown();
  return 0;
}
