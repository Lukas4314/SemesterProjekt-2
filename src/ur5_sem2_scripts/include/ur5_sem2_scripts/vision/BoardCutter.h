#pragma once
#include <opencv2/opencv.hpp>
class BoardCutter
{

private:
	cv::Point2i oldGreenPoint;
	cv::Point2i oldRedPoint;
	cv::Point2i greenPointCenter;
	cv::Point2i redPointCenter;
public:
	BoardCutter();
	~BoardCutter();

	cv::Mat cutBoard(cv::Mat image, cv::Mat greenCircle, cv::Mat redCircle, cv::Mat mask, double scale, int mode);
	std::array<std::array<double, 4>, 4> getTFchess();
	static void zoom(cv::Mat, cv::Mat&, double, cv::Point2i);


};

