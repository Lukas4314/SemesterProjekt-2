#pragma once
#include <opencv2/core.hpp>

class ImageDrawer
{
public:
	ImageDrawer();
	~ImageDrawer();
	static void drawChessBoard(cv::Mat, cv::Mat&);
};

