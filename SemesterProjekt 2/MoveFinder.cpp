#include "MoveFinder.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "ImageFinder.h"

MoveFinder::MoveFinder(){}
MoveFinder::~MoveFinder() {}


int MoveFinder::findMove(cv::Mat oldChessBoard, cv::Mat newChessBoard) {
	cv::Mat diffBoard;

	cv::resize(oldChessBoard, oldChessBoard, newChessBoard.size());

	ImageFinder::showHSVImageDifferences(newChessBoard.clone(), oldChessBoard.clone());
	cv::absdiff(oldChessBoard, newChessBoard, diffBoard);

	cv::imshow("diffBoard", diffBoard);


	return 0;
}