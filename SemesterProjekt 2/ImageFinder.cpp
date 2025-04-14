#include "ImageFinder.h"
#include <opencv2/opencv.hpp>
#include <iostream>
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
        findImageInImage(greenCircle, frame, greenBallPoint, mask, "test", bgrMode);


        cv::rectangle(frame, cv::Rect(greenBallPoint.x, greenBallPoint.y, greenCircle.cols, greenCircle.rows), cv::Scalar(0, 255, 0), 2);


        // Show the original frame
        cv::imshow("Frame", frame);

        // Exit if 'q' is pressed
        if (cv::waitKey() == 'q') break;


    }

    cap.release();  // Release webcam resource
}

void ImageFinder::findImageInImage(cv::Mat image, cv::Mat frame, cv::Point2i& point, cv::Mat mask, std::string name, int mode) {
    if (frame.empty() || image.empty()) return;

    // Match the template using the hue channel with a mask
    cv::Mat result;
    int templateModes = cv::TM_SQDIFF_NORMED;



    if (!mask.empty() && mask.size() == image.size()) {
        if (mode == ImageFinder::bgrMode) {
            cv::matchTemplate(frame, image, result, templateModes, mask);
        }
        else if (mode == ImageFinder::hsvMode) {
            cv::Mat hsvImage;
            cv::Mat hsvFrame;
            cv::cvtColor(image, hsvImage, cv::COLOR_BGR2HSV);
            cv::cvtColor(frame, hsvFrame, cv::COLOR_BGR2HSV);

            std::vector<cv::Mat> hsvImageChannels;
            std::vector<cv::Mat> hsvFrameChannels;

            cv::split(hsvImage, hsvImageChannels);
            cv::split(hsvFrame, hsvFrameChannels);


            cv::matchTemplate(hsvFrameChannels[0], hsvImageChannels[0], result, templateModes, mask);


        }
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

void ImageFinder::showHSVChannelDifferences(const cv::Mat& image, const cv::Scalar& bgrColor) {
    if (image.empty()) {
        std::cerr << "Error: Image is empty!" << std::endl;
        return;
    }

    // Convert input image to HSV
    cv::Mat hsvImage;
    cv::cvtColor(image, hsvImage, cv::COLOR_BGR2HSV);

    // Convert input color to HSV
    cv::Mat colorMat(1, 1, CV_8UC3, bgrColor);
    cv::cvtColor(colorMat, colorMat, cv::COLOR_BGR2HSV);
    cv::resize(colorMat, colorMat, cv::Size(image.cols, image.rows), cv::INTER_NEAREST);

    std::vector<cv::Mat> hsvColorChannels;
    cv::split(colorMat, hsvColorChannels);


    // Split the HSV channels of the image
    std::vector<cv::Mat> hsvChannels;
    cv::split(hsvImage, hsvChannels);

    // Compute absolute differences
    cv::Mat diffH, diffS, diffV;
    cv::absdiff(hsvChannels[0], hsvColorChannels[0], diffH);
    cv::absdiff(hsvChannels[1], hsvColorChannels[1], diffS);
    cv::absdiff(hsvChannels[2], hsvColorChannels[2], diffV);

    std::cout << hsvChannels[0].size() << "  " << hsvColorChannels[0].size() << std::endl;



    // Normalize to 8-bit for better visualization
    cv::normalize(diffH, diffH, 0, 255, cv::NORM_MINMAX);
    cv::normalize(diffS, diffS, 0, 255, cv::NORM_MINMAX);
    cv::normalize(diffV, diffV, 0, 255, cv::NORM_MINMAX);

    // Show the results

    cv::Size size = cv::Size(1440 / 3, 400);

    cv::resize(diffH, diffH, size, cv::INTER_LINEAR);
    cv::resize(diffS, diffS, size, cv::INTER_LINEAR);
    cv::resize(diffV, diffV, size, cv::INTER_LINEAR);
    cv::resize(hsvChannels[2], hsvChannels[2], size, cv::INTER_LINEAR);
    cv::resize(hsvChannels[1], hsvChannels[1], size, cv::INTER_LINEAR);


    cv::imshow("Hue Difference", diffH);
    cv::imshow("Saturation Difference", diffS);
    cv::imshow("Value Difference", diffV);
    cv::imshow("Value Channel", hsvChannels[2]);
    cv::imshow("Saturation Channel", 255 - hsvChannels[1]);


    cv::moveWindow("Hue Difference", 0, 0);
    cv::moveWindow("Saturation Difference", 0, 400);
    cv::moveWindow("Value Difference", 0, 800);
    cv::moveWindow("Value Channel", 480, 0);
    cv::moveWindow("Saturation Channel", 480, 400);


}



void ImageFinder::hueHeatmapWithParams(cv::Mat inputImage, cv::Mat& outputImage, int minSat, int maxSat, int minVal, int maxVal) {
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