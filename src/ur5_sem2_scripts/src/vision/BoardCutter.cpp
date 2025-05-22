#include "BoardCutter.h"
#include <opencv2/opencv.hpp>
#include "ImageFinder.h"

BoardCutter::BoardCutter()
{
	oldGreenPoint = cv::Point2i(0, 0);
	oldRedPoint = cv::Point2i(0, 0);
	this->name = "BoardCutter";
}
BoardCutter::~BoardCutter()
{
}

BoardCutter::BoardCutter(std::string name)
{
	this->name = name;
	oldGreenPoint = cv::Point2i(0, 0);
	oldRedPoint = cv::Point2i(0, 0);
}

cv::Mat BoardCutter::cutBoard(cv::Mat cheesWithMarkedCornors, cv::Mat greenCircle, cv::Mat redCircle, cv::Mat mask, double scale, int mode)
{
	imageSize = cv::Point2i(cheesWithMarkedCornors.cols, cheesWithMarkedCornors.rows);
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

	if (cv::norm(greenPoint - oldGreenPoint) < 10 && cv::norm(redPoint - oldRedPoint) < 10)
	{
		greenPoint = oldGreenPoint;
		redPoint = oldRedPoint;
	}
	else
	{
		oldGreenPoint = greenPoint;
		oldRedPoint = redPoint;
	}

	greenCenterPoint = cv::Point2i(greenPoint.x + greenCircle.cols / 2, greenPoint.y + greenCircle.rows / 2);
	redCenterPoint = cv::Point2i(redPoint.x + redCircle.cols / 2, redPoint.y + redCircle.rows / 2);
	greenPointCenter = greenCenterPoint;
	redPointCenter = redCenterPoint;
	TFScale = (mask.rows * scale);

	cv::Point2i difference = redCenterPoint - greenCenterPoint;

	double angle = atan2(difference.y, difference.x) * 180 / 3.14159265 - 45 - 90 - 180;
	cv::rectangle(chessWithMarkedCornorsDebug, cv::Rect(greenPoint.x, greenPoint.y, greenCircle.cols, greenCircle.rows), cv::Scalar(0, 0, 255), 2);
	cv::rectangle(chessWithMarkedCornorsDebug, cv::Rect(redPoint.x, redPoint.y, greenCircle.cols, greenCircle.rows), cv::Scalar(0, 255, 0), 2);

	// cv::imshow("MarkedConorsDebugWithMarkedCornors", chessWithMarkedCornorsDebug);

	cv::Mat rotationMatrix = cv::getRotationMatrix2D(cv::Point2i(cheesWithMarkedCornors.cols / 2, cheesWithMarkedCornors.rows / 2), angle, 1);

	cv::Rect boundingBox(
		std::min(greenCenterPoint.x, redCenterPoint.x) + (mask.rows) / 2,
		std::min(greenCenterPoint.y, redCenterPoint.y) + (mask.rows) / 2,
		abs(abs(greenCenterPoint.x - redCenterPoint.x) - (mask.rows)),
		abs(abs(greenCenterPoint.y - redCenterPoint.y) - (mask.rows)));
	cv::rectangle(chessWithMarkedCornorsDebug, boundingBox, cv::Scalar(255, 255, 0), 2);
	//std::cout << boundingBox << std::endl;
	cv::imshow("preRotationStuff" + name, chessWithMarkedCornorsDebug);

	cv::warpAffine(cheesWithMarkedCornors, cheesWithMarkedCornors, rotationMatrix, cheesWithMarkedCornors.size());
	cv::warpAffine(chessWithMarkedCornorsDebug, chessWithMarkedCornorsDebug, rotationMatrix, chessWithMarkedCornorsDebug.size());

	ImageFinder::rotatePoint(greenCenterPoint, cv::Point2i(cheesWithMarkedCornors.cols / 2, cheesWithMarkedCornors.rows / 2), -angle);
	ImageFinder::rotatePoint(redCenterPoint, cv::Point2i(cheesWithMarkedCornors.cols / 2, cheesWithMarkedCornors.rows / 2), -angle);

	cv::imshow("PostRotation" + name, chessWithMarkedCornorsDebug);

	cv::Rect boundingBox2(
		std::min(greenCenterPoint.x, redCenterPoint.x) + (mask.rows) / 2,
		std::min(greenCenterPoint.y, redCenterPoint.y) + (mask.rows) / 2,
		abs(abs(greenCenterPoint.x - redCenterPoint.x) - (mask.rows)),
		abs(abs(greenCenterPoint.y - redCenterPoint.y) - (mask.rows)));
	cv::rectangle(chessWithMarkedCornorsDebug, boundingBox, cv::Scalar(0, 255, 0), 2);

	if (boundingBox2.x < 0 || boundingBox2.y < 0 || boundingBox2.x + boundingBox.width >= cheesWithMarkedCornors.cols || boundingBox.y + boundingBox.height >= cheesWithMarkedCornors.rows)
	{
		std::cout << "Error: Bounding box out of bounds" << std::endl;
		std::cout << "Green Center point: " << greenCenterPoint << std::endl;
		std::cout << "Red Center point " << redCenterPoint << std::endl;

		std::cout << std::min(greenCenterPoint.x, redCenterPoint.x) + mask.rows / 2 << std::endl;
		std::cout << std::min(greenCenterPoint.y, redCenterPoint.y) + mask.rows / 2 << std::endl;
		std::cout << abs(abs(greenCenterPoint.x - redCenterPoint.x) - mask.rows) << std::endl;
		std::cout << abs(abs(greenCenterPoint.y - redCenterPoint.y) - mask.rows) << std::endl;

		return cheesWithMarkedCornors;
	}
	cheesWithMarkedCornors = cheesWithMarkedCornors(boundingBox2);
	return cheesWithMarkedCornors;
}

std::array<std::array<double, 4>, 4> BoardCutter::getTFchess(int mode)
{
	// Initialize the transformation matrix
	std::array<std::array<double, 4>, 4> TFchess = {{{1, 0, 0, 0},
													 {0, 1, 0, 0},
													 {0, 0, 1, 0},
													 {0, 0, 0, 1}}};
	// Set the transformation values based on the chessboard rotation and position
	cv::Point2i difference = redPointCenter - greenPointCenter;

	// Calculate the angle of rotation in degrees

	double angle = -atan2(difference.y, difference.x) * 180 / M_PI - 45;

	// Calculate the translation values
	cv::Point2i translationFromCorner = cv::Point2i(greenPointCenter.x, greenPointCenter.y);

	TFchess[0][0] = cos(angle * M_PI / 180);
	TFchess[0][1] = -sin(angle * M_PI / 180);
	TFchess[1][0] = sin(angle * M_PI / 180);
	TFchess[1][1] = cos(angle * M_PI / 180);
	TFchess[0][3] = translationFromCorner.x;
	TFchess[1][3] = translationFromCorner.y;

	if (mode == BUTTOMLEFTMODE)
	{
		std::cout << "Translation y: " << translationFromCorner.y << std::endl;
		TFchess[1][3] = imageSize.y - translationFromCorner.y;
	}

	return TFchess;
}

float BoardCutter::getAngle()
{
	// Set the transformation values based on the chessboard rotation and position
	cv::Point2i difference = redPointCenter - greenPointCenter;

	return -atan2(difference.y, difference.x) * 180 / M_PI - 45;
}

void BoardCutter::zoom(cv::Mat inputImage, cv::Mat &outputImage, double zoomFactor, cv::Point2i offset)
{
	int centerX = inputImage.cols / 2 + offset.x;
	int centerY = inputImage.rows / 2 + offset.y;
	int newWidth = static_cast<int>(inputImage.cols / zoomFactor);
	int newHeight = static_cast<int>(inputImage.rows / zoomFactor);

	// Crop the region of interest (ROI)
	cv::Rect roi(centerX - newWidth / 2, centerY - newHeight / 2, newWidth, newHeight);
	outputImage = inputImage(roi);
}

void BoardCutter::getCornorPoints(cv::Mat cheesWithMarkedCornors, cv::Mat greenCircle, cv::Mat redCircle, cv::Mat mask, double scale, int mode)
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

	if (cv::norm(greenPoint - oldGreenPoint) < 10 && cv::norm(redPoint - oldRedPoint) < 10)
	{
		greenPoint = oldGreenPoint;
		redPoint = oldRedPoint;
	}
	else
	{
		oldGreenPoint = greenPoint;
		oldRedPoint = redPoint;
	}

	greenCenterPoint = cv::Point2i(greenPoint.x + greenCircle.cols / 2, greenPoint.y + greenCircle.rows / 2);
	redCenterPoint = cv::Point2i(redPoint.x + redCircle.cols / 2, redPoint.y + redCircle.rows / 2);
	greenPointCenter = greenCenterPoint;
	redPointCenter = redCenterPoint;
	TFScale = scale;
}