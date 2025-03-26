#include "MoveFinder.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "ImageFinder.h"
#include <vector>
MoveFinder::MoveFinder(){}
MoveFinder::~MoveFinder() {}


int MoveFinder::findMove(cv::Mat oldChessBoard, cv::Mat newChessBoard, int depth = 0) {
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

	int diffBoardArray[8][8];


	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			cv::Rect square = cv::Rect(j * squareWidth, i * squareHeight, squareWidth, squareHeight);
			cv::Mat squareImage = diffBoard(square);
			int diff = cv::sum(squareImage)[0];
			diffBoardArray[i][j] = diff;
		}
	}



    std::vector<std::tuple<int, int, int>> elements;

    // Populate vector with array values and indices
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            elements.push_back({diffBoardArray[i][j], i, j});
        }
    }

    // Sort in descending order (max first)
    std::sort(elements.rbegin(), elements.rend());

    // Ensure depth doesn't exceed available elements
    int size = elements.size();
    if (depth >= size - 1) depth = size - 2;

    // Extract required indices based on depth
    auto [row1, col1] = std::make_pair(std::get<1>(elements[0]), std::get<2>(elements[0]));  // Highest
    auto [row2, col2] = std::make_pair(std::get<1>(elements[depth + 1]), std::get<2>(elements[depth + 1]));  // Depth-based second

    // Return calculated integer
    return row1 * 1000 + col1 * 100 + row2 * 10 + col2;
}