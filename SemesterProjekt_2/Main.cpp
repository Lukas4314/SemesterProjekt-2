#define NOMINMAX
#include "HueDifferenceProcessor.h"
#include "ImageFinder.h"
#include <iostream>
// #include <windows.h>
#include <opencv2/opencv.hpp>
#include <algorithm>
#include "boardCutter.h"
#include "ImageDrawer.h"

int main() {

	//cv::Mat cheesWithMarkedCornors = cv::imread("chessWithMarkedCornors2.jpg", cv::IMREAD_COLOR);
	cv::Mat greenCircle = cv::imread("greenCircle.png", cv::IMREAD_COLOR);
	cv::Mat redCircle = cv::imread("redCircle.png", cv::IMREAD_COLOR);
	cv::Mat blueCircle = cv::imread("blueCircle.png", cv::IMREAD_COLOR);
	cv::Mat mask = cv::imread("circleMask.png", cv::IMREAD_GRAYSCALE);


	//cv::cvtColor(greenCircle, greenCircle, cv::COLOR_BGR2HSV);
	//cv::cvtColor(redCircle, redCircle, cv::COLOR_BGR2HSV);
	//cv::cvtColor(blueCircle, blueCircle, cv::COLOR_BGR2HSV);



	cv::VideoCapture cap(1, cv::CAP_DSHOW);
	if (!cap.isOpened()) {
		throw std::runtime_error("Error: Could not open the webcam.");
	}
	cap.set(cv::CAP_PROP_FRAME_WIDTH, 1920);
	cap.set(cv::CAP_PROP_FRAME_HEIGHT, 1080);

	cv::Mat cheesWithMarkedCornors;

	while (true) {
		cap >> cheesWithMarkedCornors;

		//cv::imshow("Original frame", cheesWithMarkedCornors);



		cv::Point2i boundingBoxStart = cv::Point2i(550, 220);
		cv::Rect boundingBox = cv::Rect(boundingBoxStart.x, boundingBoxStart.y, 780, cheesWithMarkedCornors.rows - boundingBoxStart.y - 300);
		cheesWithMarkedCornors = cheesWithMarkedCornors(boundingBox);

		cv::imshow("cutted frame", cheesWithMarkedCornors);




		cv::Vec3b pixel = greenCircle.at<cv::Vec3b>(0, 0); // Get the first pixel (row=0, col=0)
		cv::Scalar firstPixelColor(pixel[0], pixel[1], pixel[2]); // Convert to Scalar (B, G, R
		ImageFinder::showHSVChannelDifferences(cheesWithMarkedCornors, firstPixelColor);


		if (cv::waitKey(1) == 'q') {
			break;
		}
		continue;





		// Cut out chessboard
		cv::Mat chessBoard = BoardCutter::cutBoard(cheesWithMarkedCornors, greenCircle, redCircle, mask, 0.5, ImageFinder::hsvMode);


		cv::Mat drawedChessboard = chessBoard.clone();
		ImageDrawer::drawChessBoard(chessBoard, drawedChessboard);

		//cv::imshow("Chessboard", chessBoard);
		//cv::imshow("drawedChessboard", drawedChessboard);

		// Exit if 'q' is pressed
		if (cv::waitKey(1) == 'q') {
			break;
		}
	}
}