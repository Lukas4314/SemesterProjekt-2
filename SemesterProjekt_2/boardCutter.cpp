#include "BoardCutter.h"
#include <opencv2/opencv.hpp>
#include "ImageFinder.h"

BoardCutter::BoardCutter()
{
}
BoardCutter::~BoardCutter()
{
}


cv::Mat BoardCutter::cutBoard(cv::Mat cheesWithMarkedCornors, cv::Mat greenCircle, cv::Mat redCircle, cv::Mat mask, double scale, int mode)
{
	cv::Mat chessWithMarkedCornorsDebug = cheesWithMarkedCornors.clone();
	cv::resize(cheesWithMarkedCornors, cheesWithMarkedCornors, cv::Size(), 1, 1, cv::INTER_LINEAR);
	cv::resize(greenCircle, greenCircle, cv::Size(), scale, scale, cv::INTER_LINEAR);
	cv::resize(redCircle, redCircle, cv::Size(), scale, scale, cv::INTER_LINEAR);
	cv::resize(mask, mask, cv::Size(), scale, scale, cv::INTER_LINEAR);

	cv::Point2i greenPoint(0, 0);
	cv::Point2i greenCenterPoint(0, 0);

	cv::Point2i redPoint(0, 0);
	cv::Point2i redCenterPoint(0, 0);

	ImageFinder::findImageInImage(greenCircle, cheesWithMarkedCornors, greenPoint, mask, "greenCircle", mode);
	ImageFinder::findImageInImage(redCircle, cheesWithMarkedCornors, redPoint, mask, "redCirlce", mode);


	greenCenterPoint = cv::Point2i(greenPoint.x + greenCircle.cols / 2, greenPoint.y + greenCircle.rows / 2);
	redCenterPoint = cv::Point2i(redPoint.x + redCircle.cols / 2, redPoint.y + redCircle.rows / 2);

	cv::Point2i difference = redCenterPoint - greenCenterPoint;

	double angle = atan2(difference.y, difference.x) * 180 / 3.14159265 + 45;
	std::cout << angle << std::endl;
	cv::rectangle(chessWithMarkedCornorsDebug, cv::Rect(greenPoint.x, greenPoint.y, greenCircle.cols, greenCircle.rows), cv::Scalar(0, 0, 255), 2);
	cv::rectangle(chessWithMarkedCornorsDebug, cv::Rect(redPoint.x, redPoint.y, greenCircle.cols, greenCircle.rows), cv::Scalar(0, 255, 0), 2);

	cv::imshow("MarkedConorsDebug", chessWithMarkedCornorsDebug);


	cv::Mat rotationMatrix = cv::getRotationMatrix2D(cv::Point2i(cheesWithMarkedCornors.cols / 2, cheesWithMarkedCornors.rows / 2), angle, 1);

	cv::warpAffine(cheesWithMarkedCornors, cheesWithMarkedCornors, rotationMatrix, cheesWithMarkedCornors.size());
	cv::warpAffine(chessWithMarkedCornorsDebug, chessWithMarkedCornorsDebug, rotationMatrix, chessWithMarkedCornorsDebug.size());


	ImageFinder::rotatePoint(greenCenterPoint, cv::Point2i(cheesWithMarkedCornors.cols / 2, cheesWithMarkedCornors.rows / 2), -angle);
	ImageFinder::rotatePoint(redCenterPoint, cv::Point2i(cheesWithMarkedCornors.cols / 2, cheesWithMarkedCornors.rows / 2), -angle);



	cv::Rect boundingBox(
		std::min(greenCenterPoint.x, redCenterPoint.x) + mask.rows / 2,
		std::min(greenCenterPoint.y, redCenterPoint.y) + mask.rows / 2,
		abs(greenCenterPoint.x - redCenterPoint.x) - mask.rows,
		abs(greenCenterPoint.y - redCenterPoint.y) - mask.rows
	);
	cv::rectangle(chessWithMarkedCornorsDebug, boundingBox, cv::Scalar(255, 255, 0), 2);
	//std::cout << boundingBox << std::endl;
	cv::imshow("MarkedConorsDebug", chessWithMarkedCornorsDebug);



	if (boundingBox.x < 0 || boundingBox.y < 0 || boundingBox.x + boundingBox.width > cheesWithMarkedCornors.cols || boundingBox.y + boundingBox.height > cheesWithMarkedCornors.rows) {
		return cheesWithMarkedCornors;
	}
	cheesWithMarkedCornors = cheesWithMarkedCornors(boundingBox);
	return cheesWithMarkedCornors;
}


void BoardCutter::zoom(cv::Mat inputImage, cv::Mat& outputImage, double zoomFactor, cv::Point2i offset) {
	int centerX = inputImage.cols / 2 + offset.x;
	int centerY = inputImage.rows / 2 + offset.y;
	int newWidth = static_cast<int>(inputImage.cols / zoomFactor);
	int newHeight = static_cast<int>(inputImage.rows / zoomFactor);

	// Crop the region of interest (ROI)
	cv::Rect roi(centerX - newWidth / 2, centerY - newHeight / 2, newWidth, newHeight);
	outputImage = inputImage(roi);
}