#include "MoveFinder.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "ImageFinder.h"
#include <vector>
#include "Utill.h"
#include "ImageDrawer.h"
#include <math.h>

MoveFinder::MoveFinder() {}
MoveFinder::~MoveFinder() {}

int MoveFinder::findMove(cv::Mat oldChessBoard, cv::Mat newChessBoard, int depth = 0)
{
	if (newChessBoard.cols < 8 || newChessBoard.rows < 8)
	{
		std::cout << "Error: The new chessboard is too small." << std::endl;
		return -1;
	}
	cv::Mat diffBoard;

	cv::resize(oldChessBoard, oldChessBoard, newChessBoard.size());

	ImageFinder::showHSVImageDifferences(newChessBoard.clone(), oldChessBoard.clone(), "diffBords");
	cv::absdiff(oldChessBoard, newChessBoard, diffBoard);
	cv::imshow("difBoard", diffBoard);
	cv::cvtColor(diffBoard, diffBoard, cv::COLOR_BGR2GRAY);
	int imageWidth = diffBoard.cols;
	int imageHeight = diffBoard.rows;
	int squareWidth = imageWidth / 8;
	int squareHeight = imageHeight / 8;

	cv::imshow("diffBoardGrayscale", diffBoard);

	int diffBoardArray[8][8];

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			cv::Rect square = cv::Rect(j * squareWidth, i * squareHeight, squareWidth, squareHeight);
			cv::Mat squareImage = diffBoard(square);

			cv::Mat squareImageModified = squareImage.clone();

			int diff = 0;
			int rows = squareImage.rows;
			int cols = squareImage.cols;

			for (int i = 0; i < rows; i++)
			{
				for (int j = 0; j < cols; j++)
				{
					// Get the pixel value at (i, j)
					cv::Vec3b pixel = squareImage.at<cv::Vec3b>(i, j);
					int pixelValue = pixel[0] + pixel[1] + pixel[2]; // Sum of BGR channels

					float wight = abs(i - rows / 2) * abs(j - cols / 2);
					
					diff += pixelValue * pow(wight, 2); // Weighted by distance from center
				}
			}

			diffBoardArray[i][j] = diff;
		}
	}

	// Vector to store {change value, row, col}
	std::vector<std::tuple<int, int, int>> tileChanges;

	// Populate the vector
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			tileChanges.emplace_back(diffBoardArray[i][j], i, j);
		}
	}

	// Sort individual tiles in descending order based on their change value
	std::sort(tileChanges.rbegin(), tileChanges.rend());

	// Vector to store all tile pair combinations {combined score, row1, col1, row2, col2}
	std::vector<std::tuple<int, int, int, int, int>> tilePairs;

	// Generate all possible tile pairs
	for (size_t a = 0; a < tileChanges.size(); a++)
	{
		for (size_t b = a + 1; b < tileChanges.size(); b++)
		{
			int scoreA = std::get<0>(tileChanges[a]);
			int rowA = std::get<1>(tileChanges[a]);
			int colA = std::get<2>(tileChanges[a]);

			int scoreB = std::get<0>(tileChanges[b]);
			int rowB = std::get<1>(tileChanges[b]);
			int colB = std::get<2>(tileChanges[b]);

			int combinedScore = scoreA + scoreB;

			tilePairs.emplace_back(combinedScore, rowA, colA, rowB, colB);
		}
	}

	int castleWeight = 1.5;
	int castleScoreWQ = (diffBoardArray[7][0] + diffBoardArray[7][2] + diffBoardArray[7][3] + diffBoardArray[7][4]) * castleWeight;
	int castleScoreWK = (diffBoardArray[7][5] + diffBoardArray[7][6] + diffBoardArray[7][4] + diffBoardArray[7][7]) * castleWeight;
	int castleScoreBQ = (diffBoardArray[0][0] + diffBoardArray[0][2] + diffBoardArray[0][3] + diffBoardArray[0][4]) * castleWeight;
	int castleScoreBK = (diffBoardArray[0][5] + diffBoardArray[0][6] + diffBoardArray[0][4] + diffBoardArray[0][7]) * castleWeight;

	std::cout << "castleScoreWQ: " << castleScoreWQ << std::endl;
	std::cout << "castleScoreWK: " << castleScoreWK << std::endl;
	std::cout << "castleScoreBQ: " << castleScoreBQ << std::endl;
	std::cout << "castleScoreBK: " << castleScoreBK << std::endl;
	tilePairs.emplace_back(castleScoreWQ, 0, 0, 0, Utill::WQ);
	tilePairs.emplace_back(castleScoreWK, 0, 0, 0, Utill::WK);
	tilePairs.emplace_back(castleScoreBQ, 0, 0, 0, Utill::BQ);
	tilePairs.emplace_back(castleScoreBK, 0, 0, 0, Utill::BK);

	// Sort tile pairs by combined change score in descending order
	std::sort(tilePairs.rbegin(), tilePairs.rend());

	std::cout << "actualMoveScore = " << std::get<0>(tilePairs[depth]) << std::endl;
	std::cout << "actualMoveRow1 = " << std::get<1>(tilePairs[depth]) << std::endl;
	std::cout << "actualMoveCol1 = " << std::get<2>(tilePairs[depth]) << std::endl;
	std::cout << "actualMoveRow2 = " << std::get<3>(tilePairs[depth]) << std::endl;
	std::cout << "actualMoveCol2 = " << std::get<4>(tilePairs[depth]) << std::endl;
	std::cout << "first tile score = " << diffBoardArray[std::get<1>(tilePairs[depth])][std::get<2>(tilePairs[depth])] << std::endl;
	std::cout << "second tile score = " << diffBoardArray[std::get<3>(tilePairs[depth])][std::get<4>(tilePairs[depth])] << std::endl;

	return std::get<1>(tilePairs[depth]) * 1000 + std::get<2>(tilePairs[depth]) * 100 + std::get<3>(tilePairs[depth]) * 10 + std::get<4>(tilePairs[depth]);
}