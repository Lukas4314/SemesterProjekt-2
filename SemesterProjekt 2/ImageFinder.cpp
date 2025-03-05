#include "ImageFinder.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <windows.h>
#include <string>

ImageFinder::ImageFinder() {}

void ImageFinder::testStart() {
    // Open webcam once
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        throw std::runtime_error("Error: Could not open the cam.");
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
        findImageInImage(greenCircle, frame, greenBallPoint, mask, "test");


		cv::rectangle(frame, cv::Rect(greenBallPoint.x, greenBallPoint.y, greenCircle.cols, greenCircle.rows), cv::Scalar(0, 255, 0), 2);


        // Show the original frame
        cv::imshow("Frame", frame);

        // Exit if 'q' is pressed
        if (cv::waitKey() == 'q') break;


    }

    cap.release();  // Release webcam resource
}

void ImageFinder::findImageInImage(cv::Mat image, cv::Mat frame, cv::Point2i &point, cv::Mat mask, std::string name) {
    if (frame.empty() || image.empty()) return;

    // Match the template using the hue channel with a mask
    cv::Mat result;
    int templateModes = cv::TM_SQDIFF_NORMED;
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


    cv::normalize(result, result, 0, 255, cv::NORM_MINMAX, CV_8U);
    //cv::imshow(name, 255-result);


    // Store best match coordinates
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


void ImageFinder::hueHeatmapWithParams(cv::Mat inputImage, cv::Mat& outputImage,int minSat, int maxSat, int minVal, int maxVal) {
    cv::Mat hsvImage;
    cv::cvtColor(inputImage, hsvImage, cv::COLOR_BGR2HSV);

    std::vector<cv::Mat> channels;
    cv::split(hsvImage, channels);
    cv::Mat hue = channels[0];  // Extract Hue channel
    cv::Mat saturation = channels[1];  // Extract Saturation channel
    cv::Mat value = channels[2];  // Extract Value channel

    // Create mask for low/high saturation or value
    cv::Mat mask;
    cv::Mat lowSat = saturation < minSat;
    cv::Mat highSat = saturation > maxSat;
    cv::Mat lowVal = value < minVal;
    cv::Mat highVal = value > maxVal;

    mask = lowSat | highSat | lowVal | highVal; // Combine all conditions

    // Convert Hue channel to a heatmap
    cv::Mat heatmap;
    cv::applyColorMap(hue, heatmap, cv::COLORMAP_JET);

    for (int y = 0; y < heatmap.rows; y++) {
        for (int x = 0; x < heatmap.cols; x++) {
            if (mask.at<uchar>(y, x) > 0) {
                heatmap.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 0);
            }
        }
    }

    outputImage = heatmap;
}
