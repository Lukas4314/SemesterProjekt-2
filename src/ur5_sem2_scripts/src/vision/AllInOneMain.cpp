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

int AllInOneMain::getPieceMoved(int depth)
{

	if (depth != 0)
	{
		if (oldChessboard.empty() || chessboard.empty())
		{
			std::cout << "Old chessboard or chessboard is empty" << std::endl;
			return 0;
		}
		int move = MoveFinder::findMove(oldChessboard.clone(), chessboard.clone(), depth);
		return move;
	}

	std::cout << "gets new images" << std::endl;
	if (!chessboard.empty())
	{
		oldChessboard = chessboard.clone();
	}

	cap >> chessWithMarkedCornors;
	cap >> chessWithMarkedCornors;
	cap >> chessWithMarkedCornors;
	cap >> chessWithMarkedCornors;
	cap >> chessWithMarkedCornors;
	cap >> chessWithMarkedCornors;
	cap >> chessWithMarkedCornors;

	// cv::imshow("Original frame", chessWithMarkedCornors);


	/*
	cv::Point2i boundingBoxStart = cv::Point2i(550, 220);
	cv::Rect boundingBox = cv::Rect(boundingBoxStart.x, boundingBoxStart.y, 780, chessWithMarkedCornors.rows - boundingBoxStart.y - 300);
	std::cout << chessWithMarkedCornors.size() << std::endl;
	chessWithMarkedCornors = chessWithMarkedCornors(boundingBox);

	*/


	chessWithMarkedCornors = boardCutter2.cutBoard(chessWithMarkedCornors, greenCircle, yellowCircle, mask, 1, ImageFinder::hsvMode2);


	// cv::Vec3b pixel = greenCircle.at<cv::Vec3b>(0, 0);		  // Get the first pixel (row=0, col=0)
	// cv::Scalar firstPixelColor(pixel[0], pixel[1], pixel[2]); // Convert to Scalar (B, G, R
	// ImageFinder::showHSVChannelDifferences(chessWithMarkedCornors, firstPixelColor);

	// Cut out chessboard
	chessboard = boardCutter.cutBoard(chessWithMarkedCornors, greenCircle, redCircle, mask, 0.5, ImageFinder::hsvMode2);

	cv::Mat drawedChessboard = chessboard.clone();
	ImageDrawer::drawChessBoard(chessboard, drawedChessboard);

	cv::imshow("drawedChessboard", drawedChessboard);
	cv::imshow("chessBoard", chessboard.clone());

	int movedPiece = 0;
	if (!oldChessboard.empty())
	{
		cv::imshow("oldBoard", oldChessboard);
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

std::string AllInOneMain::getPieceMovedString(int depth)
{
	std::string move = Utill::translateIntMoveToString(getPieceMoved(depth));
	return move;
}

BoardCutter AllInOneMain::getBoardCutter()
{
	return boardCutter;
}