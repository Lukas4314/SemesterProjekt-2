#pragma once
#include <opencv2/opencv.hpp>
#include <string>

class ImageFinder
{
public:
	ImageFinder();
	void testStart();
	static void findImageInImage(cv::Mat, cv::Mat, cv::Point2i&, cv::Mat, std::string);
	static void rotatePoint(cv::Point2i& point, cv::Point2i center, double angle);

	static void hueHeatmapWithParams(cv::Mat inputImage, cv::Mat &outputImage, int minSat, int maxSat, int minVal, int maxVal);


	~ImageFinder();
};

