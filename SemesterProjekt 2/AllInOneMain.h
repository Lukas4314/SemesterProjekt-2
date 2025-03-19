#include <opencv2/opencv.hpp>
#include "boardCutter.h"

#pragma once
class AllInOneMain
{
public:
	AllInOneMain();
	~AllInOneMain();
	int getPieceMoved();
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

