#pragma once
#include <opencv2/opencv.hpp>
class BoardCutter
{
public:
	BoardCutter();
	~BoardCutter();

	static cv::Mat cutBoard(cv::Mat image, cv::Mat greenCircle, cv::Mat redCircle, cv::Mat mask, double scale, int mode);
	static void zoom(cv::Mat, cv::Mat&, double, cv::Point2i);


};

