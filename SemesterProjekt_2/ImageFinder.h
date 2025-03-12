#pragma once
#include <opencv2/opencv.hpp>
#include <string>

class ImageFinder
{
public:
	const static int bgrMode = 0;
	const static int hsvMode = 1;

	ImageFinder();
	void testStart();
	static void findImageInImage(cv::Mat, cv::Mat, cv::Point2i&, cv::Mat, std::string, int mode);
	static void rotatePoint(cv::Point2i& point, cv::Point2i center, double angle);

	static void hueHeatmapWithParams(cv::Mat inputImage, cv::Mat &outputImage, int minSat, int maxSat, int minVal, int maxVal);

	static void showHSVChannelDifferences(const cv::Mat& image, const cv::Scalar& bgrColor);


	~ImageFinder();
};

