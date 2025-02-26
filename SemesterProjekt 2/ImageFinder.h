#pragma once
#include <opencv2/opencv.hpp>


class ImageFinder
{
public:
	ImageFinder();
	void testStart();
	void findImageInImage(cv::Mat, cv::Mat, cv::Point2i&, cv::Mat);
	cv::Mat extractHueChannel(const cv::Mat& frame);
	void rotatePoint(cv::Point2i& point, cv::Point2i center, double angle);

	~ImageFinder();
};

