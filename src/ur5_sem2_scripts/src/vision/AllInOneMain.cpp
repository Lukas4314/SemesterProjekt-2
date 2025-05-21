#include "AllInOneMain.h"
#include "ImageFinder.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <algorithm>
#include "BoardCutter.h"
#include "ImageDrawer.h"
#include "MoveFinder.h"
#include "Utill.h"
#include <string>
#include "ament_index_cpp/get_package_share_directory.hpp"
#include "ur5_sem2_scripts/logger/Logger.h"

AllInOneMain::AllInOneMain(int camera_index)
{
	std::string package_share_dir = ament_index_cpp::get_package_share_directory("ur5_sem2_scripts");

	greenCircle = cv::imread(package_share_dir + "/pictures/greenCircle.png", cv::IMREAD_COLOR);
	redCircle = cv::imread(package_share_dir + "/pictures/redCircle.png", cv::IMREAD_COLOR);
	blueCircle = cv::imread(package_share_dir + "/pictures/blueCircle.png", cv::IMREAD_COLOR);
	yellowCircle = cv::imread(package_share_dir + "/pictures/yellowCircle.png", cv::IMREAD_COLOR);
	mask = cv::imread(package_share_dir + "/pictures/circleMask.png", cv::IMREAD_GRAYSCALE);

	// cv::cvtColor(greenCircle, greenCircle, cv::COLOR_BGR2HSV);
	// cv::cvtColor(redCircle, redCircle, cv::COLOR_BGR2HSV);
	// cv::cvtColor(blueCircle, blueCircle, cv::COLOR_BGR2HSV);

	cap.open(camera_index, cv::CAP_ANY);
	if (!cap.isOpened())
	{
		throw std::runtime_error("Error: Could not open the webcam.");
	}
	cap.set(cv::CAP_PROP_FRAME_WIDTH, 1920);
	cap.set(cv::CAP_PROP_FRAME_HEIGHT, 1080);
	boardCutter = BoardCutter();
	boardCutter2 = BoardCutter("BoardCutter2");
}
AllInOneMain::~AllInOneMain() {}

int AllInOneMain::getPieceMoved(int depth, const std::string calledBy)
{

	if (depth != 0 && calledBy == CAMERA)
	{
		if (oldChessboard.empty() || chessboard.empty())
		{
			std::cout << "Old chessboard or chessboard is empty" << std::endl;
			return 0;
		}
		int move = MoveFinder::findMove(oldChessboard.clone(), chessboard.clone(), depth);
		return move;
	}

	if (!chessboard.empty())
	{
		oldChessboard = chessboard.clone();
	}

	flushCamera();

	cv::imshow("Original frame" + calledBy, chessWithMarkedCornors);

	// Get the different colors
	cv::Vec3b yellowCircleColor = yellowCircle.at<cv::Vec3b>(0, 0);
	cv::Vec3b redCircleColor = redCircle.at<cv::Vec3b>(0, 0);
	cv::Vec3b greenCircleColor = greenCircle.at<cv::Vec3b>(0, 0);

	// Convert to Scalar (B, G, R)
	cv::Scalar yellowCircleScalar(yellowCircleColor[0], yellowCircleColor[1], yellowCircleColor[2]);
	cv::Scalar redCircleScalar(redCircleColor[0], redCircleColor[1], redCircleColor[2]);
	cv::Scalar greenCircleScalar(greenCircleColor[0], greenCircleColor[1], greenCircleColor[2]);

	ImageFinder::showHSVChannelDifferences(chessWithMarkedCornors, redCircleScalar, "redColorPegSearch" + calledBy);
	ImageFinder::showHSVChannelDifferences(chessWithMarkedCornors, yellowCircleScalar, "YellowColorPegSearch" + calledBy);

	float circleScale2 = 1.4;
	chessWithMarkedCornors = boardCutter2.cutBoard(chessWithMarkedCornors, yellowCircle, redCircle, mask, circleScale2, ImageFinder::hsvMode2);

	cv::imshow("chessWithMarkedCornors after plok cut" + calledBy, chessWithMarkedCornors.clone());

	// Show the differences for the chessboard
	ImageFinder::showHSVChannelDifferences(chessWithMarkedCornors, greenCircleScalar, "greenColorBoardSearch" + calledBy);
	ImageFinder::showHSVChannelDifferences(chessWithMarkedCornors, redCircleScalar, "redColorBoardSearch" + calledBy);

	// Cut out chessboard
	chessboard = boardCutter.cutBoard(chessWithMarkedCornors, greenCircle, redCircle, mask, 0.45, ImageFinder::hsvMode2);
	cv::Mat rotationMatrix = cv::getRotationMatrix2D(cv::Point2i(chessboard.cols / 2, chessboard.rows / 2), 180, 1);
	cv::warpAffine(chessboard, chessboard, rotationMatrix, chessboard.size());

	cv::Mat drawedChessboard = chessboard.clone();
	ImageDrawer::drawChessBoard(chessboard, drawedChessboard);

	cv::imshow("drawedChessboard" + calledBy, drawedChessboard);
	cv::imshow("chessBoard" + calledBy, chessboard.clone());

	if (calledBy == ENGINE)
	{
		return 0;
	}

	int movedPiece = 0;
	if (!oldChessboard.empty())
	{
		cv::imshow("oldBoard" + calledBy, oldChessboard);
		movedPiece = MoveFinder::findMove(oldChessboard.clone(), chessboard.clone(), depth);
	}

	if (movedPiece != 0)
	{
		std::cout << "Moved piece: " << movedPiece << std::endl;
		return movedPiece;
	}
	else
	{
		return 0;
	}

	return 0;
}

std::string AllInOneMain::getPieceMovedString(int depth, const std::string calledBy)
{
	std::string move = Utill::translateIntMoveToString(getPieceMoved(depth, calledBy));
	return move;
}

BoardCutter AllInOneMain::getBoardCutter(int index)
{
	if (index == 0)
	{
		return boardCutter;
	}
	else if (index == 1)
	{
		return boardCutter2;
	}
	else
	{
		std::cout << "Invalid index" << std::endl;
		return boardCutter;
	}
}

void AllInOneMain::flushCamera()
{
	for (int i = 0; i < 10; i++)
	{
		cap >> chessWithMarkedCornors;
	}
}