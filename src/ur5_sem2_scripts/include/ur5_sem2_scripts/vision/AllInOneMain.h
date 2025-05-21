#include <opencv2/opencv.hpp>
#include "BoardCutter.h"
#include <string>
#pragma once
class AllInOneMain
{
public:
	AllInOneMain(int camera_index);
	~AllInOneMain();
	int getPieceMoved(int depth = 0);
	std::string getPieceMovedString(int depth = 0);
	BoardCutter getBoardCutter(int index);
	void flushCamera();
private:
	cv::VideoCapture cap;
	cv::Mat greenCircle;
	cv::Mat redCircle;
	cv::Mat blueCircle;
	cv::Mat yellowCircle;
	cv::Mat mask;
	cv::Mat chessWithMarkedCornors;
	cv::Mat oldChessboard;
	cv::Mat chessboard;
	BoardCutter boardCutter;
	BoardCutter boardCutter2;


};

