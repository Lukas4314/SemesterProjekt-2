#include "ImageDrawer.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

ImageDrawer::ImageDrawer() {}
ImageDrawer::~ImageDrawer() {}



void ImageDrawer::drawChessBoard(cv::Mat chessBoard, cv::Mat& output) {
	for (int i = 1; i < 8; i++) {
		cv::line(output, cv::Point(i * chessBoard.cols / 8, 0), cv::Point(i * chessBoard.cols / 8, chessBoard.rows), cv::Scalar(0, 0, 255), 2);
		cv::line(output, cv::Point(0, i * chessBoard.rows / 8), cv::Point(chessBoard.cols, i * chessBoard.rows / 8), cv::Scalar(0, 0, 255), 2);
	}
}