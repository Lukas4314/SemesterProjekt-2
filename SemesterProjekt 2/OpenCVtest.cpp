#include "HueDifferenceProcessor.h"
#include "ImageFinder.h"
#include <iostream>
#include <windows.h>

int main() {
	cv::Mat cheesWithMarkedCornors = cv::imread("chessWithMarkedCornors.jpg", cv::IMREAD_COLOR);
	cv::Mat greenCircle = cv::imread("greenCircle.png", cv::IMREAD_COLOR);
	ImageFinder imageFinder;
	int x = 0;
	int y = 0;
	//imageFinder.findImageInImage(greenCircle, cheesWithMarkedCornors,x, y);
	//std::cout << "x: " << x << " y: " << y << std::endl;
	imageFinder.testStart();
	cv::waitKey(0);
}