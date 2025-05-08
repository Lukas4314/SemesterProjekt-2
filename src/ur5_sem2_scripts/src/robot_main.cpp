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



void printMatrix(const std::array<std::array<double, 4>, 4>& matrix, auto logger)
{
  for (size_t i = 0; i < 4; ++i)
  {
    std::string row_str;
    for (size_t j = 0; j < 4; ++j)
    {
      row_str += std::to_string(matrix[i][j]) + " ";
    }
    RCLCPP_INFO(logger, "%s", row_str.c_str());
  }
}


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
  node->get_parameter("camera_index", camera_index); // Retrieve the parameter

  ChessMoves chessMoves(node);
  chessMoves.move_to_idle();

  AllInOneMain allInOneMain = AllInOneMain(camera_index);
  allInOneMain.getPieceMovedString(0);



  // Types in the preknown yellow point for callibrating the offset to the table cornor
  cv::Point2f preKnownYellowPointInCm(17.5, 57.5);
  double pixelPerCm = 11.2;

  // Gets the transformation matrix form the topleft of the original image to the yellow plok
  std::array<std::array<double, 4>, 4> cam_table_T_pixel = allInOneMain.getBoardCutter(1).getTFchess(TOPLEFTMODE);

  // Makes it into cm
  std::array<std::array<double, 4>, 4> cam_table_T_cm = cam_table_T_pixel;
  cam_table_T_cm[0][3] = cam_table_T_pixel[0][3] / pixelPerCm;
  cam_table_T_cm[1][3] = cam_table_T_pixel[1][3] / pixelPerCm;

  // substract the preknown yellow point in cm from the transformation matrix to achieve an transformation matrix form the cam to the table cornor top left
  cam_table_T_cm[0][3] = cam_table_T_cm[0][3] - preKnownYellowPointInCm.x;
  cam_table_T_cm[1][3] = cam_table_T_cm[1][3] - preKnownYellowPointInCm.y;

  // Makes the transformation matrix from the table to the yellow plok with its rotation and then adds the translation
	std::array<std::array<double, 4>, 4> table_yellowPlok_T = {{
		{1, 0, 0, 0},
		{0, -1, 0, 0},
		{0, 0, -1, 0},
		{0, 0, 0, 1}
	}};
  table_yellowPlok_T[0][3] = preKnownYellowPointInCm.x;
  table_yellowPlok_T[1][3] = preKnownYellowPointInCm.y;

  // Makes the transformation matrix from the cam to the yellow plok
  std::array<std::array<double, 4>, 4> yellowPlok_boardGreen_T_pixels = allInOneMain.getBoardCutter(0).getTFchess(BUTTOMLEFTMODE);
  
  std::array<std::array<double, 4>, 4> yellowPlok_boardGreen_T_cm = yellowPlok_boardGreen_T_pixels;

  // Makes it into cm
  yellowPlok_boardGreen_T_cm[0][3] = yellowPlok_boardGreen_T_pixels[0][3] / pixelPerCm;
  yellowPlok_boardGreen_T_cm[1][3] = yellowPlok_boardGreen_T_pixels[1][3] / pixelPerCm;


  // Adds the 2.5 cm to the x and y for the square offset where the image is cutted to
  yellowPlok_boardGreen_T_cm[0][3] = yellowPlok_boardGreen_T_cm[0][3] + 2.5;
  yellowPlok_boardGreen_T_cm[1][3] = yellowPlok_boardGreen_T_cm[1][3] + 2.5;


  std::array<std::array<double, 4>, 4> base_yellowPlok_T_cm = {{
    {0, 1, 0, 35},
    {-1, 0, 0, 25},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
  }};
  
  // Makes the transformation matrix from the base to the chessboard
  std::array<std::array<double, 4>, 4> base_boardGreen_T_cm = BoardTransformer::multiplyMatrices(base_yellowPlok_T_cm, yellowPlok_boardGreen_T_cm);
  
  std::array<std::array<double, 4>, 4> base_boardGreen_T_m = {{
    {{base_boardGreen_T_cm[0][0], base_boardGreen_T_cm[0][1], base_boardGreen_T_cm[0][2], base_boardGreen_T_cm[0][3]/100.0}},
    {{base_boardGreen_T_cm[1][0], base_boardGreen_T_cm[1][1], base_boardGreen_T_cm[1][2], base_boardGreen_T_cm[1][3]/100.0}},
    {{base_boardGreen_T_cm[2][0], base_boardGreen_T_cm[2][1], base_boardGreen_T_cm[2][2], base_boardGreen_T_cm[2][3]}},
    {{base_boardGreen_T_cm[3][0], base_boardGreen_T_cm[3][1], base_boardGreen_T_cm[3][2], base_boardGreen_T_cm[3][3]}}
  }};
  






  
  moveStruct move;
  move.piece = 'p';
  move.type = 'm';
  move.start[0] = 0;
  move.start[1] = 0;
  move.end[0] = 7;
  move.end[1] = 7;
  move.captured = '-';
  move.color = 'w';

  moveStruct move1;
  move1 = move;
  move1.start[0] = 7;
  move1.start[1] = 7;
  move1.end[0] = 7;
  move1.end[1] = 0;
  move1.captured = 'b';

  moveStruct move2;
  move2 = move1;
  move2.captured = 'q';

  moveStruct move3;
  move3 = move2;
  move3.captured = 'k';

  moveStruct move4;
  move4 = move;
  move4.type = 'a';
  move4.piece = 'k';
  move4.color = 'b';


  double raw_TF[4][4];
  for (size_t i = 0; i < 4; ++i)
  {
    for (size_t j = 0; j < 4; ++j)
    {
      raw_TF[i][j] = base_boardGreen_T_m[i][j];
    }
  }


  // output the transformation matrix

  RCLCPP_INFO(logger, "cam_table_T_cm:");
  printMatrix(cam_table_T_cm, logger);

  RCLCPP_INFO(logger, "yellowPlok_boardGreen_T_cm:");
  printMatrix(yellowPlok_boardGreen_T_cm, logger);

  RCLCPP_INFO(logger, "base_boardGreen_T:");
  printMatrix(base_boardGreen_T_m, logger);


  //cv::waitKey(0);
  chessMoves.move(move, raw_TF);
  chessMoves.move(move1, raw_TF);
  chessMoves.move(move2, raw_TF);
  chessMoves.move(move3, raw_TF);
  chessMoves.move(move4, raw_TF);


  // Shutdown ROS
  rclcpp::shutdown();
  return 0;
}



