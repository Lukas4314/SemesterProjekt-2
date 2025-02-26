#define NOMINMAX
#include "HueDifferenceProcessor.h"
#include "ImageFinder.h"
#include <iostream>
#include <windows.h>
#include <opencv2/opencv.hpp>
#include <algorithm>
#include "boardCutter.h"

int main() {
	cv::Mat cheesWithMarkedCornors = cv::imread("chessWithMarkedCornors2.jpg", cv::IMREAD_COLOR);
	cv::Mat greenCircle = cv::imread("greenCircle.png", cv::IMREAD_COLOR);
	cv::Mat redCircle = cv::imread("redCircle.png", cv::IMREAD_COLOR);
	cv::Mat mask = cv::imread("circleMask.png", cv::IMREAD_GRAYSCALE);

	cv::Mat chessBoard = boardCutter::cutBoard(cheesWithMarkedCornors, greenCircle, redCircle, mask);
	cv::imshow("Chess Board", chessBoard);



	cv::waitKey(0);

}