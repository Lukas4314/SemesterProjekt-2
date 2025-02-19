#include "ImageFinder.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <windows.h>

ImageFinder::ImageFinder() {}

void ImageFinder::testStart() {
    // Open webcam once
    cv::VideoCapture cap(1);
    if (!cap.isOpened()) {
        throw std::runtime_error("Error: Could not open the webcam.");
    }

    // Load reference images
    cv::Mat greenCircle = cv::imread("greenCircle.png", cv::IMREAD_COLOR);
    cv::Mat redCircle = cv::imread("redCircle.png", cv::IMREAD_COLOR);

    if (greenCircle.empty() || redCircle.empty()) {
        throw std::runtime_error("Error: Could not load the reference images.");
    }

    int greenBallx = 0, greenBally = 0;

    cv::namedWindow("Webcam", cv::WINDOW_AUTOSIZE);

    while (true) {
		std::cout << "inside loop" << std::endl;

        cv::Mat frame;
        cap >> frame;  // Capture frame
        if (frame.empty()) break;

        // Find green circle in the frame
        findImageInImage(greenCircle, frame, greenBallx, greenBally);


		cv::rectangle(frame, cv::Rect(greenBallx, greenBally, greenCircle.cols, greenCircle.rows), cv::Scalar(0, 255, 0), 2);


        // Show the original frame
        cv::imshow("Webcam", frame);

        // Exit if 'q' is pressed
        if (cv::waitKey(1) == 'q') break;


    }

    cap.release();  // Release webcam resource
}

void ImageFinder::findImageInImage(cv::Mat image, cv::Mat frame, int& x, int& y) {
    if (frame.empty() || image.empty()) return;

    // Convert images to HSV
    cv::Mat frameHSV, imageHSV;
    cv::cvtColor(frame, frameHSV, cv::COLOR_BGR2HSV);
    cv::cvtColor(image, imageHSV, cv::COLOR_BGR2HSV);

    // Match the template using hue channel (channel index 0)
    cv::Mat result;
    cv::matchTemplate(frameHSV, imageHSV, result, cv::TM_SQDIFF_NORMED);

    // Find best match location
    double minVal, maxVal;
    cv::Point minLoc, maxLoc;
    cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);

    // Store best match coordinates
    x = minLoc.x;
    y = minLoc.y;
}

ImageFinder::~ImageFinder() {}
