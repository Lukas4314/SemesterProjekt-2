#pragma once
#include <opencv2/opencv.hpp>


class ImageFinder
{
public:
	ImageFinder();
	void testStart();
	void findImageInImage(cv::Mat, cv::Mat, int&, int&);
	cv::Mat extractHueChannel(const cv::Mat& frame);

	~ImageFinder();
};

