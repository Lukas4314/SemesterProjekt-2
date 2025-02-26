#define NOMINMAX
#include "HueDifferenceProcessor.h"
#include "ImageFinder.h"
#include <iostream>
#include <windows.h>
#include <opencv2/opencv.hpp>
#include <algorithm>
#include "boardCutter.h"


int main() {

	//cv::Mat cheesWithMarkedCornors = cv::imread("chessWithMarkedCornors2.jpg", cv::IMREAD_COLOR);
	cv::Mat greenCircle = cv::imread("greenCircle.png", cv::IMREAD_COLOR);
	cv::Mat redCircle = cv::imread("redCircle.png", cv::IMREAD_COLOR);
	cv::Mat mask = cv::imread("circleMask.png", cv::IMREAD_GRAYSCALE);


	cv::VideoCapture cap(1, cv::CAP_DSHOW);
	if (!cap.isOpened()) {
		throw std::runtime_error("Error: Could not open the webcam.");
	}
	cap.set(cv::CAP_PROP_FRAME_WIDTH, 1920);
	cap.set(cv::CAP_PROP_FRAME_HEIGHT, 1080);


	cv::Mat cheesWithMarkedCornors;

    while (true) {
        cap >> cheesWithMarkedCornors;


        // Apply zoom
        BoardCutter::zoom(cheesWithMarkedCornors, cheesWithMarkedCornors, 2.5, cv::Point2i(-40, 100));

        // Cut out chessboard
        cv::Mat chessBoard = BoardCutter::cutBoard(cheesWithMarkedCornors, greenCircle, redCircle, mask, 0.5);
		//ImageFinder::hueHeatmapWithParams(chessBoard, chessBoard, 30, 255, 30, 255);

		
		cv::imshow("Chessboard", chessBoard);
        // Exit if 'q' is pressed
        if (cv::waitKey(1) == 'q') {
            break;
        }
    }
}