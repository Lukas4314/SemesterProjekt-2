#include "HueDifferenceProcessor.h"
#include <opencv2/opencv.hpp>
#include <iostream>

HueDifferenceProcessor::HueDifferenceProcessor() {
    cap.open(0);
    if (!cap.isOpened()) {
        throw std::runtime_error("Error: Could not open the webcam.");
    }
    lastFrame = captureFrame();
    lastHue = extractHueChannel(lastFrame);
    cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);
}

HueDifferenceProcessor::~HueDifferenceProcessor() {
    cap.release();
    cv::destroyAllWindows();
}

cv::Mat HueDifferenceProcessor::captureFrame() {
    cv::Mat frame;
    cap >> frame;
    return frame;
}

cv::Mat HueDifferenceProcessor::extractHueChannel(const cv::Mat& frame) {
    cv::Mat hsvFrame, hueChannel;
    cv::cvtColor(frame, hsvFrame, cv::COLOR_BGR2HSV);
    cv::extractChannel(hsvFrame, hueChannel, 0);
    return hueChannel;
}

cv::Mat HueDifferenceProcessor::computeHueDifference(const cv::Mat& hue1, const cv::Mat& hue2) {
    cv::Mat diff;
    cv::absdiff(hue1, hue2, diff);
    cv::normalize(diff, diff, 0, 255, cv::NORM_MINMAX);
    cv::cvtColor(diff, diff, cv::COLOR_GRAY2BGR);
    return diff;
}

cv::Mat HueDifferenceProcessor::resizeToWidth(const cv::Mat& img, int newWidth) {
    int newHeight = static_cast<int>((static_cast<double>(newWidth) / img.cols) * img.rows);
    cv::Mat resizedImg;
    cv::resize(img, resizedImg, cv::Size(newWidth, newHeight), 0, 0, cv::INTER_AREA);
    return resizedImg;
}

void HueDifferenceProcessor::processFrames() {
    while (true) {
        if (cv::waitKey() == 'q') {
            break;
        }

        cv::Mat currentFrame = captureFrame();
        if (currentFrame.empty()) {
            std::cerr << "Error: Captured empty frame." << std::endl;
            break;
        }

        cv::Mat currentHue = extractHueChannel(currentFrame);
        cv::Mat diffFrame = computeHueDifference(currentHue, lastHue);

        std::vector<cv::Mat> frames = { currentFrame, lastFrame, diffFrame };
        cv::Mat finalFrame;
        cv::hconcat(frames, finalFrame);
        finalFrame = resizeToWidth(finalFrame, 1536);

        cv::imshow(windowName, finalFrame);

        lastFrame = currentFrame.clone();
        lastHue = currentHue.clone();
    }
}
