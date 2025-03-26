#include <opencv2/opencv.hpp>
#include "BoardCutter.h"
#include <string>
#pragma once
class AllInOneMain
{
public:
	AllInOneMain();
	~AllInOneMain();
	int getPieceMoved();
	std::string getPieceMovedString();

private:
	cv::VideoCapture cap;
	cv::Mat greenCircle;
	cv::Mat redCircle;
	cv::Mat blueCircle;
	cv::Mat mask;
	cv::Mat cheesWithMarkedCornors;
	cv::Mat oldChessboard;
	BoardCutter boardCutter;


};

