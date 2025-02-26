#pragma once
#include <opencv2/opencv.hpp>
class boardCutter
{
public:
	boardCutter();
	~boardCutter();

	static cv::Mat cutBoard(cv::Mat image, cv::Mat greenCircle, cv::Mat redCircle, cv::Mat mask);



};

