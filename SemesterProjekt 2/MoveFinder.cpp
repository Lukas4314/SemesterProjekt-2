#include "MoveFinder.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "ImageFinder.h"

MoveFinder::MoveFinder(){}
MoveFinder::~MoveFinder() {}


int MoveFinder::findMove(cv::Mat oldChessBoard, cv::Mat newChessBoard) {
	if (newChessBoard.cols < 8 || newChessBoard.rows < 8) {
		std::cout << "Error: The new chessboard is too small." << std::endl;
		return -1;
	}
	cv::Mat diffBoard;

	cv::resize(oldChessBoard, oldChessBoard, newChessBoard.size());

	ImageFinder::showHSVImageDifferences(newChessBoard.clone(), oldChessBoard.clone());
	cv::absdiff(oldChessBoard, newChessBoard, diffBoard);
	cv::cvtColor(diffBoard, diffBoard, cv::COLOR_BGR2GRAY);
	int imageWidth = diffBoard.cols;
	int imageHeight = diffBoard.rows;
	int squareWidth = imageWidth / 8;
	int squareHeight = imageHeight / 8;

	cv::Point2i highestDiffPoint = cv::Point2i(0, 0);
	cv::Point2i secondHighestDiffPoint = cv::Point2i(0, 0);
	int highestDiff = 0;
	int secondHighestDiff = 0;


	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			cv::Rect square = cv::Rect(j * squareWidth, i * squareHeight, squareWidth, squareHeight);
			cv::Mat squareImage = diffBoard(square);
			int diff = cv::sum(squareImage)[0];
			if (diff > highestDiff) {
				secondHighestDiff = highestDiff;
				secondHighestDiffPoint = highestDiffPoint;
				highestDiff = diff;
				highestDiffPoint = cv::Point2i(j, i);
			}
			else if (diff > secondHighestDiff) {
				secondHighestDiff = diff;
				secondHighestDiffPoint = cv::Point2i(j, i);
			}
		}
	}

	//std::cout << "Highest diff point: " << highestDiffPoint << "  Second highest diff point" << secondHighestDiffPoint << std::endl;


	cv::imshow("diffBoard", diffBoard);

	
	return highestDiffPoint.x * 1000 + highestDiffPoint.y*100 + secondHighestDiffPoint.x * 10 + secondHighestDiffPoint.y;
}