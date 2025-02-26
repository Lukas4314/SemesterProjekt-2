#include "ImageFinder.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <windows.h>

ImageFinder::ImageFinder() {}

void ImageFinder::testStart() {
    // Open webcam once
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        throw std::runtime_error("Error: Could not open the webcam.");
    }

    // Load reference images
    cv::Mat greenCircle = cv::imread("greenCircle.png", cv::IMREAD_COLOR);
    cv::Mat redCircle = cv::imread("redCircle.png", cv::IMREAD_COLOR);
    cv::Mat mask = cv::imread("circleMask.png", cv::IMREAD_GRAYSCALE);
    float imageScale = 0.5;
    cv::resize(greenCircle, greenCircle, cv::Size(), imageScale, imageScale, cv::INTER_LINEAR);
    cv::resize(redCircle, redCircle, cv::Size(), imageScale, imageScale, cv::INTER_LINEAR);
    cv::resize(mask, mask, cv::Size(), imageScale, imageScale, cv::INTER_LINEAR);



    if (greenCircle.empty() || redCircle.empty()) {
        throw std::runtime_error("Error: Could not load the reference images.");
    }

	cv::Point2i greenBallPoint(0, 0);

    cv::namedWindow("Webcam", cv::WINDOW_AUTOSIZE);

    while (true) {
        cv::Mat frame;
        cap >> frame;  // Capture frame
        if (frame.empty()) break;

        // Find green circle in the frame
        findImageInImage(greenCircle, frame, greenBallPoint, mask);


		cv::rectangle(frame, cv::Rect(greenBallPoint.x, greenBallPoint.y, greenCircle.cols, greenCircle.rows), cv::Scalar(0, 255, 0), 2);


        // Show the original frame
        cv::imshow("Webcam", frame);

        // Exit if 'q' is pressed
        if (cv::waitKey() == 'q') break;


    }

    cap.release();  // Release webcam resource
}

void ImageFinder::findImageInImage(cv::Mat image, cv::Mat frame, cv::Point2i &point, cv::Mat mask) {
    if (frame.empty() || image.empty()) return;

    // Match the template using the hue channel with a mask
    cv::Mat result;
    int templateModes = cv::TM_SQDIFF;
    if (!mask.empty() && mask.size() == image.size()) {
        cv::matchTemplate(frame, image, result, templateModes, mask);
    }
    else {
		std::cout << "No mask" << std::endl;
        cv::matchTemplate(frame, image, result, templateModes);
    }

    // Find the best match location
    double minVal, maxVal;
    cv::Point minLoc, maxLoc;
    cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);

    // Store best match coordinates
	std::cout << "minLoc.x: " << minLoc.x << " minLoc.y: " << minLoc.y << std::endl;
	point = minLoc;
}

void ImageFinder::rotatePoint(cv::Point2i& point, cv::Point2i center, double angle) {
	double s = sin(angle * CV_PI / 180);
	double c = cos(angle * CV_PI / 180);
	// Translate point back to origin
    point.x -= center.x;
	point.y -= center.y;
	// Rotate point
	int xnew = (int)round(point.x * c - point.y * s);
	int ynew = (int)round(point.x * s + point.y * c);
	// Translate point back
	point.x = xnew + center.x;
	point.y = ynew + center.y;
}
ImageFinder::~ImageFinder() {}
