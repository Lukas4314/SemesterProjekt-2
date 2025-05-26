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
#include "ur5_sem2_scripts/vision/VisionInterface.h"
#include "ur5_sem2_scripts/BoardTransformer.hpp"
#include "ur5_sem2_scripts/board/ChessBoard.h"
#include "ur5_sem2_scripts/BoardTransformer.hpp"
#include "ur5_sem2_scripts/vision/Utill.h"
#include "ur5_sem2_scripts/logger/Logger.h"

void printMatrix(const std::array<std::array<double, 4>, 4> &matrix, auto logger)
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

std::array<std::array<double, 4>, 4> getTransformationMatrixYellowPegToGreenSpot(VisionInterface &visionInterface)
{
  double pixelPerCm = 10.945696;

  // Makes the transformation matrix from the cam to the yellow plok
  std::array<std::array<double, 4>, 4> yellowPlok_boardGreen_T_pixels = visionInterface.getBoardCutter(0).getTFchess(BUTTOMLEFTMODE);
  Logger::setValue(ANGLE_OF_TRANSFORMATION_MATRIX_PEGS, std::to_string(visionInterface.getBoardCutter(0).getAngle()));
  Logger::setValue(ANGLE_OF_TRANSFORMATION_MATRIX_CHESSBOARD, std::to_string(visionInterface.getBoardCutter(1).getAngle()));

  std::array<std::array<double, 4>, 4> yellowPlok_boardGreen_T_cm = yellowPlok_boardGreen_T_pixels;

  // Makes it into cm
  yellowPlok_boardGreen_T_cm[0][3] = yellowPlok_boardGreen_T_pixels[0][3] / pixelPerCm;
  yellowPlok_boardGreen_T_cm[1][3] = yellowPlok_boardGreen_T_pixels[1][3] / pixelPerCm;

  // Adds the 2.5 cm to the x and y for the square offset where the image is cutted to
  float pegRadius = 1.6;
  yellowPlok_boardGreen_T_cm[0][3] = yellowPlok_boardGreen_T_cm[0][3] + pegRadius;
  yellowPlok_boardGreen_T_cm[1][3] = yellowPlok_boardGreen_T_cm[1][3] + pegRadius;
  return yellowPlok_boardGreen_T_cm;
}

std::array<std::array<double, 4>, 4> getTransformationMatrixBaseToGreenspot(VisionInterface &visionInterface)
{
  std::array<std::array<double, 4>, 4> yellowPlok_boardGreen_T_cm = getTransformationMatrixYellowPegToGreenSpot(visionInterface);

  std::array<std::array<double, 4>, 4> base_yellowPlok_T_cm = {{{0, 1, 0, 35},
                                                                {-1, 0, 0, 25},
                                                                {0, 0, 1, 0},
                                                                {0, 0, 0, 1}}};

  // Makes the transformation matrix from the base to the chessboard
  std::array<std::array<double, 4>, 4> base_boardGreen_T_cm = BoardTransformer::multiplyMatrices(base_yellowPlok_T_cm, yellowPlok_boardGreen_T_cm);

  std::array<std::array<double, 4>, 4> base_boardGreen_T_m = {{{{base_boardGreen_T_cm[0][0], base_boardGreen_T_cm[0][1], base_boardGreen_T_cm[0][2], base_boardGreen_T_cm[0][3] / 100.0}},
                                                               {{base_boardGreen_T_cm[1][0], base_boardGreen_T_cm[1][1], base_boardGreen_T_cm[1][2], base_boardGreen_T_cm[1][3] / 100.0}},
                                                               {{base_boardGreen_T_cm[2][0], base_boardGreen_T_cm[2][1], base_boardGreen_T_cm[2][2], base_boardGreen_T_cm[2][3]}},
                                                               {{base_boardGreen_T_cm[3][0], base_boardGreen_T_cm[3][1], base_boardGreen_T_cm[3][2], base_boardGreen_T_cm[3][3]}}}};
  return base_boardGreen_T_m;
}

std::array<std::array<double, 4>, 4> getFullTransformationMatrix(VisionInterface &visionInterface)
{
  // Create a logger
  auto logger = rclcpp::get_logger("final_main");

  std::array<std::array<double, 4>, 4> base_boardGreen_T_m = getTransformationMatrixBaseToGreenspot(visionInterface);

  float boardSize = 0.288;
  float cali = 0.005;

  std::array<std::array<double, 4>, 4> boardGreen_boardRed_T_m = {{{0, -1, 0, boardSize + cali},
                                                                   {-1, 0, 0, boardSize + cali},
                                                                   {0, 0, -1, 0},
                                                                   {0, 0, 0, 1}}};

  std::array<std::array<double, 4>, 4> base_boardRed_T_m = BoardTransformer::multiplyMatrices(base_boardGreen_T_m, boardGreen_boardRed_T_m);

  std::array<std::array<double, 4>, 4> yellowPlok_boardGreen_T_cm = getTransformationMatrixYellowPegToGreenSpot(visionInterface);
  RCLCPP_INFO(logger, "yellowPlok_boardGreen_T_cm:");
  printMatrix(yellowPlok_boardGreen_T_cm, logger);

  RCLCPP_INFO(logger, "base_boardGreen_T_m:");
  printMatrix(base_boardGreen_T_m, logger);

  RCLCPP_INFO(logger, "boardGreen_boardRed_T_m:");
  printMatrix(boardGreen_boardRed_T_m, logger);

  RCLCPP_INFO(logger, "base_boardRed_T_m:");
  printMatrix(base_boardRed_T_m, logger);

  return base_boardRed_T_m;
}

MoveStruct applyCameraMove(VisionInterface &visionInterface, ChessBoard &chess)
{
  Logger::setValue(CAMERA_MOVE, "1");
  string move;
  bool succesMove;
  int moveDepth = 0;

  do
  {
    move = visionInterface.getPieceMovedString(moveDepth, CAMERA);
    cout << "Move is: " << move << endl;
    succesMove = chess.applyIfValidMove(move);
    cout << "Succes move: " << succesMove << endl;
    moveDepth++;

  } while (!succesMove);
  cout << "Camera Move is: " << move << endl;
  chess.printBoard();
  chess.drawBoard();

  Logger::setValue(MOVES_TRIED_BEFORE_SUCCESS, to_string(moveDepth));

  // Here it needs to get movestruct
  MoveStruct movePlan;
  movePlan = chess.getMoveStruct();

  if (movePlan.captured != '-')
  {
    movePlan.type = 'k';
  }
  else
  {
    movePlan.type = '-';
  }
  return movePlan;
}

MoveStruct applyStockfishMove(StockfishUCI &engine, ChessBoard &chess)
{
  // create a logger
  auto logger = rclcpp::get_logger("final_main");

  int depth = 15;
  vector<string> moveHistory = chess.getMoveHistory();
  cout << "Move history: " << endl;
  for (const auto &move : moveHistory)
  {
    cout << move << endl;
  }
  string allMoves = Utill::vectorStringToString(moveHistory);
  string output = engine.getBestMove(allMoves, depth);
  cout << "Output from engine: " << output << endl;

  size_t pos = output.find("bestmove ");

  if (pos == string::npos)
  {
    cerr << "Failed to find bestmove in engine output!" << endl;
  }

  size_t start = output.find("bestmove ") + 9;
  size_t end = output.find(' ', start);
  string bestMove = output.substr(start, end - start);

  // Validate the bestMove string
  if (bestMove.length() < 4 || bestMove.length() > 5)
  {
    cerr << "Invalid move received from Stockfish: " << bestMove << endl;
  }

  RCLCPP_DEBUG(logger, ("Applying engine move"));
  RCLCPP_DEBUG(logger, ("Bestmove: " + bestMove).c_str());

  char promotedTo = '-';
  if (bestMove.length() == 5)
  {
    promotedTo = bestMove.substr(4, 1)[0];
  }

  string translatedBestMove = Utill::translateEngineBestMove(bestMove);
  cout << "translatedBestmove is: " << translatedBestMove << endl;
  //--- StockfishUCI end
  chess.applyIfValidMove(translatedBestMove);
  RCLCPP_DEBUG(logger, ("Engine Move is: " + translatedBestMove).c_str());
  chess.printBoard();
  chess.drawBoard();

  // Here it needs to get movestruct
  MoveStruct movePlan;
  movePlan = chess.getMoveStruct();

  return movePlan;
}

int main(int argc, char *argv[])
{

  // Make logger for csv file
  Logger::initialize(Logger::getAllLoggerKeys());

  // Initialize ROS and create the Node
  rclcpp::init(argc, argv);
  auto const node = std::make_shared<rclcpp::Node>(
      "final_main",
      rclcpp::NodeOptions().automatically_declare_parameters_from_overrides(true));

  // Create a ROS logger
  auto const logger = rclcpp::get_logger("final_main");
  int camera_index;
  node->get_parameter("camera_index", camera_index); // Retrieve the parameter

  //
  VisionInterface visionInterface = VisionInterface(camera_index);
  visionInterface.flushCamera();

  ChessMoves chessMoves(node);
  chessMoves.move_to_idle();

  float end_effector_angle = 0.0;
  // Create a chessboard object
  ChessBoard chess;
  StockfishUCI engine;

  chess.printBoard();
  chess.drawBoard();

  // Takes start image
  visionInterface.getPieceMovedString(0, CAMERA);

  end_effector_angle = visionInterface.getBoardCutter(0).getAngle() * M_PI / 180;
  chessMoves.setEndEffectorAngle(end_effector_angle);

  std::array<std::array<double, 4>, 4> base_yellowPlok_T_cm = {{{0, 1, 0, 0.35},
                                                                {-1, 0, 0, 0.25},
                                                                {0, 0, 1, 0},
                                                                {0, 0, 0, 1}}};

  // Moves the robot to green spot on the board for calibration
  std::array<std::array<double, 4>, 4> base_boardGreen_T_m = getTransformationMatrixBaseToGreenspot(visionInterface);

  std::array<std::array<double, 4>, 4> base_boardRed_T_m = getFullTransformationMatrix(visionInterface);

  double test_TF[4][4];

  for (size_t i = 0; i < 4; ++i)
  {
    for (size_t j = 0; j < 4; ++j)
    {
      test_TF[i][j] = base_yellowPlok_T_cm[i][j];
    }
  }
  // chessMoves.moveToCenterInForTransformationMatrix(test_TF);

  for (size_t i = 0; i < 4; ++i)
  {
    for (size_t j = 0; j < 4; ++j)
    {
      test_TF[i][j] = base_boardGreen_T_m[i][j];
    }
  }
  // chessMoves.moveToCenterInForTransformationMatrix(test_TF);

  for (size_t i = 0; i < 4; ++i)
  {
    for (size_t j = 0; j < 4; ++j)
    {
      test_TF[i][j] = base_boardRed_T_m[i][j];
    }
  }
  // chessMoves.moveToCenterInForTransformationMatrix(test_TF);

  // if the player wants to be black
  float boardAngle = visionInterface.getBoardCutter(0).getAngle();
  if ((boardAngle > 90 && boardAngle < 180) || (boardAngle < -90 && boardAngle > -180))
  {
    std::array<std::array<double, 4>, 4> TF = getFullTransformationMatrix(visionInterface);

    // Puts it into the array
    double raw_TF[4][4];
    for (size_t i = 0; i < 4; ++i)
    {
      for (size_t j = 0; j < 4; ++j)
      {
        raw_TF[i][j] = TF[i][j];
      }
    }

    // Applies the move from stokfish
    MoveStruct movePlan;
    movePlan = applyStockfishMove(engine, chess);
    std::cout << "Now trying to move robot with best move " << std::endl;

    end_effector_angle = visionInterface.getBoardCutter(0).getAngle() * M_PI / 180;

    chessMoves.setEndEffectorAngle(end_effector_angle);
    chessMoves.move(movePlan, raw_TF);
    std::cout << "Moved the robot" << std::endl;
    Logger::setValue(CAMERA_MOVE, "0");
    Logger::writeRow();
  }

  bool instaQuit = false;

  char keyPressed = cv::waitKey(0);
  while (keyPressed != 32)
  {
    if (keyPressed == 'q')
    {
      instaQuit = true;
      break;
    }
    keyPressed = cv::waitKey(0);
  }
  while (!instaQuit)
  {

    // Gets a move from the camera and checks if it is valid and repeats until it is
    MoveStruct movePlanCamera = applyCameraMove(visionInterface, chess);

    chess.printBoard();
    chess.drawBoard();

    // Gets the transformation matrix
    std::array<std::array<double, 4>, 4> TF = getFullTransformationMatrix(visionInterface);

    // Puts it into the array
    double raw_TF[4][4];
    for (size_t i = 0; i < 4; ++i)
    {
      for (size_t j = 0; j < 4; ++j)
      {
        raw_TF[i][j] = TF[i][j];
      }
    }

    end_effector_angle = visionInterface.getBoardCutter(0).getAngle() * M_PI / 180;
    chessMoves.setEndEffectorAngle(end_effector_angle);
    chessMoves.move(movePlanCamera, raw_TF);

    // Writes the row before stockfish starts playing
    Logger::writeRow();

    if (chess.isMated(chess.getActiveColor()))
    {
      cout << "Checkmate! " << chess.getActiveColor() << " is mated!" << endl;
      break;
    }

    if (chess.isRemi(chess.getActiveColor()))
    {
      cout << "Remi! " << endl;
      break;
    }

    // Applies the move from stokfish
    MoveStruct movePlan;

    movePlan = applyStockfishMove(engine, chess);
    std::cout << "Now trying to move robot with best move " << std::endl;

    end_effector_angle = visionInterface.getBoardCutter(0).getAngle() * M_PI / 180;
    ;
    chessMoves.setEndEffectorAngle(end_effector_angle);
    chessMoves.move(movePlan, raw_TF);
    std::cout << "Moved the robot" << std::endl;

    // Just for opdating the camera image
    visionInterface.getPieceMovedString(0, ENGINE);

    // writes the row after stockfish has played
    Logger::writeRow();

    if (chess.isMated(chess.getActiveColor()))
    {
      cout << "Checkmate! " << chess.getActiveColor() << " is mated!" << endl;
      break;
    }

    if (chess.isRemi(chess.getActiveColor()))
    {
      cout << "Remi! " << endl;
      break;
    }

    chess.printBoard();
    chess.drawBoard();

    // Check if the user quits, or wait for the player move
    keyPressed = cv::waitKey(0);
    while (keyPressed != 32)
    {
      if (keyPressed == 'q')
      {
        instaQuit = true;
        break;
      }
      keyPressed = cv::waitKey(0);
    }
  }

  // Shutdown ROS
  rclcpp::shutdown();
  return 0;
}
