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
	std::array<std::array<double, 4>, 4> table_yellowPlot_T = {{
		{1, 0, 0, 0},
		{0, -1, 0, 0},
		{0, 0, -1, 0},
		{0, 0, 0, 1}
	}};
  table_yellowPlot_T[0][3] = preKnownYellowPointInCm.x;
  table_yellowPlot_T[1][3] = preKnownYellowPointInCm.y;

  // Makes the transformation matrix from the cam to the yellow plok
  std::array<std::array<double, 4>, 4> yellowPlot_boardGreen_T_pixels = allInOneMain.getBoardCutter(0).getTFchess(BUTTOMLEFTMODE);
  
  std::array<std::array<double, 4>, 4> yellowPlot_boardGreen_T_cm = yellowPlot_boardGreen_T_pixels;

  // Makes it into cm
  yellowPlot_boardGreen_T_cm[0][3] = yellowPlot_boardGreen_T_pixels[0][3] / pixelPerCm;
  yellowPlot_boardGreen_T_cm[1][3] = yellowPlot_boardGreen_T_pixels[1][3] / pixelPerCm;


  // Adds the 2.5 cm to the x and y for the square offset where the image is cutted to
  yellowPlot_boardGreen_T_cm[0][3] = yellowPlot_boardGreen_T_cm[0][3] + 2.5;
  yellowPlot_boardGreen_T_cm[1][3] = yellowPlot_boardGreen_T_cm[1][3] + 2.5;




  // IIIIIIIIIIIIIIIIIII

  // next thing to do is to multiply the transformation matricies to achieve the desired transformation matrix and apply it, but i dont know what it want so it is magnes task

  // IIIIIIIIIIIIII





  /*
  std::array<std::array<double, 4>, 4> TFcamPlokker = allInOneMain.getBoardCutter(1).getTFchess();
  std::array<std::array<double, 4>, 4> TFPlokkerCam = BoardTransformer::getInverse(TFcamPlokker);
  std::array<std::array<double, 4>, 4> TFPlokkerChess = BoardTransformer::multiplyMatrices(TFcamPlokker, TFcamchessCm);
  std::array<std::array<double, 4>, 4> TF = BoardTransformer::multiplyMatrices(TFPlokkerChess, chessMoves.TFRed);

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
  for (size_t i = 0; i < 4; ++i)
  {
    for (size_t j = 0; j < 4; ++j)
    {
      raw_TF[i][j] = TF[i][j];
    }
  }
*/

  // output the transformation matrix

  RCLCPP_INFO(logger, "cam_table_T_cm:");
  for (size_t i = 0; i < 4; ++i)
  {
    std::string row_str;
    for (size_t j = 0; j < 4; ++j)
    {
      row_str += std::to_string(cam_table_T_cm[i][j]) + " ";
    }
    RCLCPP_INFO(logger, "%s", row_str.c_str());
  }

  RCLCPP_INFO(logger, "yellowPlot_boardGreen_T_cm:");
  for (size_t i = 0; i < 4; ++i)
  {
    std::string row_str;
    for (size_t j = 0; j < 4; ++j)
    {
      row_str += std::to_string(yellowPlot_boardGreen_T_cm[i][j]) + " ";
    }
    RCLCPP_INFO(logger, "%s", row_str.c_str());
  }

  /*
  RCLCPP_INFO(logger, "TFPlokkerChess:");
  for (size_t i = 0; i < 4; ++i)
  {
    std::string row_str;
    for (size_t j = 0; j < 4; ++j)
    {
      row_str += std::to_string(TFPlokkerChess[i][j]) + " ";
    }
    RCLCPP_INFO(logger, "%s", row_str.c_str());
  }
  RCLCPP_INFO(logger, "TF:");
  for (size_t i = 0; i < 4; ++i)
  {
    std::string row_str;
    for (size_t j = 0; j < 4; ++j)
    {
      row_str += std::to_string(raw_TF[i][j]) + " ";
    }
    RCLCPP_INFO(logger, "%s", row_str.c_str());
  }


  */

  cv::waitKey(0);
  // chessMoves.move(move, raw_TF);

  // Shutdown ROS
  rclcpp::shutdown();
  return 0;
}
