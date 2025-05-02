#include "BoardCutter.h"
#include <opencv2/opencv.hpp>
#include "ImageFinder.h"

BoardCutter::BoardCutter()
{
	oldGreenPoint = cv::Point2i(0, 0);
	oldRedPoint = cv::Point2i(0, 0);
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


	ImageFinder::findImageInImage(redCircle, cheesWithMarkedCornors, redPoint, mask, "redCirlce", mode);
	ImageFinder::findImageInImage(greenCircle, cheesWithMarkedCornors, greenPoint, mask, "greenCircle", mode);

	if (cv::norm(greenPoint - oldGreenPoint) < 10 && cv::norm(redPoint - oldRedPoint) < 10) {
		greenPoint = oldGreenPoint;
		redPoint = oldRedPoint;
	}
	else {
		oldGreenPoint = greenPoint;
		oldRedPoint = redPoint;
	}


	greenCenterPoint = cv::Point2i(greenPoint.x + greenCircle.cols / 2, greenPoint.y + greenCircle.rows / 2);
	redCenterPoint = cv::Point2i(redPoint.x + redCircle.cols / 2, redPoint.y + redCircle.rows / 2);
	greenPointCenter = greenCenterPoint;
	redPointCenter = redCenterPoint;
	TFScale = scale;

	cv::Point2i difference = redCenterPoint - greenCenterPoint;

	double angle = atan2(difference.y, difference.x) * 180 / 3.14159265 - 45 - 90;
	cv::rectangle(chessWithMarkedCornorsDebug, cv::Rect(greenPoint.x, greenPoint.y, greenCircle.cols, greenCircle.rows), cv::Scalar(0, 0, 255), 2);
	cv::rectangle(chessWithMarkedCornorsDebug, cv::Rect(redPoint.x, redPoint.y, greenCircle.cols, greenCircle.rows), cv::Scalar(0, 255, 0), 2);

	//cv::imshow("MarkedConorsDebugWithMarkedCornors", chessWithMarkedCornorsDebug);


	cv::Mat rotationMatrix = cv::getRotationMatrix2D(cv::Point2i(cheesWithMarkedCornors.cols / 2, cheesWithMarkedCornors.rows / 2), angle, 1);

	cv::warpAffine(cheesWithMarkedCornors, cheesWithMarkedCornors, rotationMatrix, cheesWithMarkedCornors.size());
	cv::warpAffine(chessWithMarkedCornorsDebug, chessWithMarkedCornorsDebug, rotationMatrix, chessWithMarkedCornorsDebug.size());


	ImageFinder::rotatePoint(greenCenterPoint, cv::Point2i(cheesWithMarkedCornors.cols / 2, cheesWithMarkedCornors.rows / 2), -angle);
	ImageFinder::rotatePoint(redCenterPoint, cv::Point2i(cheesWithMarkedCornors.cols / 2, cheesWithMarkedCornors.rows / 2), -angle);


	cv::Rect boundingBox(
		std::min(greenCenterPoint.x, redCenterPoint.x) + mask.rows / 2,
		std::min(greenCenterPoint.y, redCenterPoint.y) + mask.rows / 2,
		abs(abs(greenCenterPoint.x - redCenterPoint.x) - mask.rows),
		abs(abs(greenCenterPoint.y - redCenterPoint.y) - mask.rows)
	);
	cv::rectangle(chessWithMarkedCornorsDebug, boundingBox, cv::Scalar(255, 255, 0), 2);
	//std::cout << boundingBox << std::endl;
	cv::imshow(name + ": " + "MarkedConorsDebug", chessWithMarkedCornorsDebug);



	if (boundingBox.x < 0 || boundingBox.y < 0 || boundingBox.x + boundingBox.width >= cheesWithMarkedCornors.cols || boundingBox.y + boundingBox.height >= cheesWithMarkedCornors.rows) {
		std::cout << "Error: Bounding box out of bounds" << std::endl;
		std::cout << "Green Center point: " << greenCenterPoint << std::endl;
		std::cout << "Red Center point " << redCenterPoint << std::endl;

		std::cout << std::min(greenCenterPoint.x, redCenterPoint.x) + mask.rows / 2 << std::endl;
		std::cout << std::min(greenCenterPoint.y, redCenterPoint.y) + mask.rows / 2 << std::endl;
		std::cout << abs(abs(greenCenterPoint.x - redCenterPoint.x) - mask.rows) << std::endl;
		std::cout << abs(abs(greenCenterPoint.y - redCenterPoint.y) - mask.rows) << std::endl;


		return cheesWithMarkedCornors;
	}
	cheesWithMarkedCornors = cheesWithMarkedCornors(boundingBox);
	return cheesWithMarkedCornors;
}

std::array<std::array<double, 4>, 4> BoardCutter::getTFchess()
{
	// Initialize the transformation matrix
	std::array<std::array<double, 4>, 4> TFchess = {{
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1}
	}};
	// Set the transformation values based on the chessboard rotation and position
	cv::Point2i difference = redPointCenter - greenPointCenter;

	// Calculate the angle of rotation in degrees

	double angle = 180 -(atan2(difference.y, difference.x) * 180 / 3.14159265 - 40);

	// Calculate the translation values
	cv::Point2i translationFromCorner = cv::Point2i(greenPointCenter.x + 550,greenPointCenter.y + 220);
	cv::Point2i translationToCorner = cv::Point2i(500.0,180.0);
	cv::Point2i translation = translationToCorner - translationFromCorner;


	TFchess[0][0] = cos(angle * M_PI / 180);
	TFchess[0][1] = -sin(angle * M_PI / 180);
	TFchess[1][0] = sin(angle * M_PI / 180);
	TFchess[1][1] = cos(angle * M_PI / 180);
	TFchess[1][3] = translation.x/1100.0 + 0.375 ;
	TFchess[0][3] = translation.y/1100.0 + 0.935 + 0.024;


	return TFchess;
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