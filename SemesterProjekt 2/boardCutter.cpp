#include "boardCutter.h"
#include <opencv2/opencv.hpp>
#include "ImageFinder.h"

boardCutter::boardCutter()
{
}
boardCutter::~boardCutter()
{
}


cv::Mat boardCutter::cutBoard(cv::Mat cheesWithMarkedCornors, cv::Mat greenCircle, cv::Mat redCircle, cv::Mat mask)
{
	double imageScale = 0.5;
	cv::resize(cheesWithMarkedCornors, cheesWithMarkedCornors, cv::Size(), 0.5, 0.5, cv::INTER_LINEAR);
	cv::resize(greenCircle, greenCircle, cv::Size(), imageScale, imageScale, cv::INTER_LINEAR);
	cv::resize(redCircle, redCircle, cv::Size(), imageScale, imageScale, cv::INTER_LINEAR);
	cv::resize(mask, mask, cv::Size(), imageScale, imageScale, cv::INTER_LINEAR);

	cv::Point2i greenPoint(0, 0);
	cv::Point2i greenCenterPoint(0, 0);

	cv::Point2i redPoint(0, 0);
	cv::Point2i redCenterPoint(0, 0);

	ImageFinder::findImageInImage(greenCircle, cheesWithMarkedCornors, greenPoint, mask);
	ImageFinder::findImageInImage(redCircle, cheesWithMarkedCornors, redPoint, mask);


	greenCenterPoint = cv::Point2i(greenPoint.x + greenCircle.cols / 2, greenPoint.y + greenCircle.rows / 2);
	redCenterPoint = cv::Point2i(redPoint.x + redCircle.cols / 2, redPoint.y + redCircle.rows / 2);

	cv::Point2i difference = redCenterPoint - greenCenterPoint;

	double angle = atan2(difference.y, difference.x) * 180 / 3.14159265 + 45;
	std::cout << "Angle: " << angle << std::endl;

	cv::rectangle(cheesWithMarkedCornors, cv::Rect(greenPoint.x, greenPoint.y, greenCircle.cols, greenCircle.rows), cv::Scalar(255, 0, 0), 2);
	cv::rectangle(cheesWithMarkedCornors, cv::Rect(redPoint.x, redPoint.y, greenCircle.cols, greenCircle.rows), cv::Scalar(255, 0, 0), 2);

	cv::Mat rotationMatrix = cv::getRotationMatrix2D(cv::Point2i(cheesWithMarkedCornors.cols / 2, cheesWithMarkedCornors.rows / 2), angle, 1);
	cv::warpAffine(cheesWithMarkedCornors, cheesWithMarkedCornors, rotationMatrix, cheesWithMarkedCornors.size());

	ImageFinder::rotatePoint(greenCenterPoint, cv::Point2i(cheesWithMarkedCornors.rows / 2, cheesWithMarkedCornors.cols / 2), angle);
	ImageFinder::rotatePoint(redCenterPoint, cv::Point2i(cheesWithMarkedCornors.rows / 2, cheesWithMarkedCornors.cols / 2), angle);

	std::cout << "Green Center Point: " << greenCenterPoint << std::endl;
	std::cout << "Red Center Point: " << redCenterPoint << std::endl;

	cv::Rect boundingBox(
		std::min(greenCenterPoint.x, redCenterPoint.x),
		std::min(greenCenterPoint.y, redCenterPoint.y) + greenCircle.rows,
		abs(greenCenterPoint.x - redCenterPoint.x),
		abs(greenCenterPoint.y - redCenterPoint.y) - greenCircle.rows * 2
	);
	cv::rectangle(cheesWithMarkedCornors, boundingBox, cv::Scalar(255, 255, 0), 2);

	cheesWithMarkedCornors = cheesWithMarkedCornors(boundingBox);
	return cheesWithMarkedCornors;
}