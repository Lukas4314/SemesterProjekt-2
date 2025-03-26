#include "AllInOneMain.h"
#include "ImageFinder.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <algorithm>
#include "BoardCutter.h"
#include "ImageDrawer.h"
#include "MoveFinder.h"
#include "Utill.h"


AllInOneMain::AllInOneMain() {  
   greenCircle = cv::imread("../greenCircle.png", cv::IMREAD_COLOR);  
   redCircle = cv::imread("../redCircle.png", cv::IMREAD_COLOR);  
   blueCircle = cv::imread("../blueCircle.png", cv::IMREAD_COLOR);  
   mask = cv::imread("../circleMask.png", cv::IMREAD_GRAYSCALE);  
 
   //cv::cvtColor(greenCircle, greenCircle, cv::COLOR_BGR2HSV);  
   //cv::cvtColor(redCircle, redCircle, cv::COLOR_BGR2HSV);  
   //cv::cvtColor(blueCircle, blueCircle, cv::COLOR_BGR2HSV);  
 
   cap.open(0, cv::CAP_ANY);  
   if (!cap.isOpened()) {  
       throw std::runtime_error("Error: Could not open the webcam.");  
   }  
   cap.set(cv::CAP_PROP_FRAME_WIDTH, 1920);  
   cap.set(cv::CAP_PROP_FRAME_HEIGHT, 1080);  
 
   boardCutter = BoardCutter();  
}
AllInOneMain::~AllInOneMain() {}

int AllInOneMain::getPieceMoved() {

	cap >> cheesWithMarkedCornors;
	cap >> cheesWithMarkedCornors;
	cap >> cheesWithMarkedCornors;
	cap >> cheesWithMarkedCornors;
	cap >> cheesWithMarkedCornors;
	cap >> cheesWithMarkedCornors;
	cap >> cheesWithMarkedCornors;


	//cv::imshow("Original frame", cheesWithMarkedCornors);



	cv::Point2i boundingBoxStart = cv::Point2i(550, 220);
	cv::Rect boundingBox = cv::Rect(boundingBoxStart.x, boundingBoxStart.y, 780, cheesWithMarkedCornors.rows - boundingBoxStart.y - 300);
	cheesWithMarkedCornors = cheesWithMarkedCornors(boundingBox);

	cv::Vec3b pixel = greenCircle.at<cv::Vec3b>(0, 0); // Get the first pixel (row=0, col=0)
	cv::Scalar firstPixelColor(pixel[0], pixel[1], pixel[2]); // Convert to Scalar (B, G, R
	//ImageFinder::showHSVChannelDifferences(cheesWithMarkedCornors, firstPixelColor);




	// Cut out chessboard
	cv::Mat chessBoard = boardCutter.cutBoard(cheesWithMarkedCornors, greenCircle, redCircle, mask, 0.5, ImageFinder::hsvMode2);
	cv::Mat drawedChessboard = chessBoard.clone();
	ImageDrawer::drawChessBoard(chessBoard, drawedChessboard);

	//cv::imshow("drawedChessboard", drawedChessboard);
	cv::imshow("chessBoard", chessBoard);

	int movedPiece = 0;
	if (!oldChessboard.empty()) {
		cv::imshow("oldBoard", oldChessboard);
		movedPiece = MoveFinder::findMove(oldChessboard, chessBoard);
	}
	oldChessboard = chessBoard.clone();

	if (movedPiece != 0) {
		return movedPiece;
	}
	else {
		return 0;
	}

	return 0;
}


std::string AllInOneMain::getPieceMovedString() {
	return Utill::translateIntMoveToString(getPieceMoved());
}