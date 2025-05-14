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
	float diffBoardArray[8][8];

	cv::Mat diffBoardModified = diffBoard.clone();

	int height = diffBoard.rows;
	int width = diffBoard.cols;

	// Grid configuration
	int cellWidth = width / 8;
	int cellHeight = height / 8;
	int falloffDistance = 5; // Distance from grid line to start dimming

	cv::Mat circularMask(squareHeight, squareWidth, CV_8UC1, cv::Scalar(0));
	cv::circle(circularMask,
           cv::Point(squareWidth / 2, squareHeight / 2),
           std::min(squareWidth, squareHeight) / 2,
           cv::Scalar(255), -1);  // Filled circle



	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			// Distance to nearest vertical grid line
			int distX = std::min(x % cellWidth, cellWidth - (x % cellWidth));
			// Distance to nearest horizontal grid line
			int distY = std::min(y % cellHeight, cellHeight - (y % cellHeight));

			int distToGrid = std::min(distX, distY);

			double scale = 1.0;
			if (distToGrid < falloffDistance)
			{
				scale = static_cast<double>(distToGrid) / falloffDistance;
			}

			uchar &pixel = diffBoardModified.at<uchar>(y, x);
			pixel = static_cast<uchar>(pixel * scale);
		}
	}

	std::cout << "Type: " << diffBoardModified.type() << ", Channels: " << diffBoardModified.channels() << std::endl;
	cv::imshow("diffBoardModified", diffBoardModified);

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			cv::Rect square = cv::Rect(j * squareWidth, i * squareHeight, squareWidth, squareHeight);
			cv::Mat squareImage = diffBoardModified(square);

			int diff = pow(cv::sum(squareImage)[0], 2) / (255 * 255);
			diffBoardArray[i][j] = diff;
		}
	}

	cv::cvtColor(oldChessBoard, oldChessBoard, cv::COLOR_BGR2GRAY);
	cv::cvtColor(newChessBoard, newChessBoard, cv::COLOR_BGR2GRAY);

	for (int i = 0; i < 8; i++)
{
    for (int j = 0; j < 8; j++)
    {
        cv::Rect square(j * squareWidth, i * squareHeight, squareWidth, squareHeight);
        cv::Mat squareOld = oldChessBoard(square);
        cv::Mat squareNew = newChessBoard(square);

        // Apply circular mask
        cv::Mat maskedOld, maskedNew;
        squareOld.copyTo(maskedOld, circularMask);
        squareNew.copyTo(maskedNew, circularMask);

        double oldSum = cv::sum(maskedOld)[0];
        double newSum = cv::sum(maskedNew)[0];
        double diff = std::abs(oldSum - newSum);

        diffBoardArray[i][j] = diff;
    }
}


	// Finds the highest difference
	int maxDiff = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (diffBoardArray[i][j] > maxDiff)
			{
				maxDiff = diffBoardArray[i][j];
			}
		}
	}


	// normalize everthing to 0-5
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			diffBoardArray[i][j] = diffBoardArray[i][j] / maxDiff * 5;
		}
	}


	// Square root of the normalized value
	float exponent = 0.33f;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			diffBoardArray[i][j] = pow(diffBoardArray[i][j], exponent);
		}
	}





	// Print the boardvalues
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			std::cout << diffBoardArray[i][j] << " ";
		}
		std::cout << std::endl;
	}


	// Vector to store {change value, row, col}
	std::vector<std::tuple<float, int, int>> tileChanges;

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
	std::vector<std::tuple<float, int, int, int, int>> tilePairs;

	// Generate all possible tile pairs
	for (size_t a = 0; a < tileChanges.size(); a++)
	{
		for (size_t b = a + 1; b < tileChanges.size(); b++)
		{
			float scoreA = std::get<0>(tileChanges[a]);
			int rowA = std::get<1>(tileChanges[a]);
			int colA = std::get<2>(tileChanges[a]);

			float scoreB = std::get<0>(tileChanges[b]);
			int rowB = std::get<1>(tileChanges[b]);
			int colB = std::get<2>(tileChanges[b]);

			float combinedScore = scoreA + scoreB;

			tilePairs.emplace_back(combinedScore, rowA, colA, rowB, colB);
		}
	}


	




	float castleWeight = 1.2f;
	float castleScoreWQ = (diffBoardArray[7][0] + diffBoardArray[7][2] + diffBoardArray[7][3] + diffBoardArray[7][4]) / castleWeight;
	float castleScoreWK = (diffBoardArray[7][5] + diffBoardArray[7][6] + diffBoardArray[7][4] + diffBoardArray[7][7]) / castleWeight;
	float castleScoreBQ = (diffBoardArray[0][0] + diffBoardArray[0][2] + diffBoardArray[0][3] + diffBoardArray[0][4]) / castleWeight;
	float castleScoreBK = (diffBoardArray[0][5] + diffBoardArray[0][6] + diffBoardArray[0][4] + diffBoardArray[0][7]) / castleWeight;

	float enPassantWeight = 1.2f;

	for (int file = 0; file < 8; file++)
	{
		// The king and queen side corresponds to the side which the pawn is taken from.
		float enPassantWQSide;
		float enPassantWKSide;
		float enPassantBQSide;
		float enPassantBKSide;
		if (file == 0)
		{
			// Since file 0 is the A column the pawn can only be taken fom the kingside since the queen side is off the board.
			enPassantWKSide = (diffBoardArray[5][file] + diffBoardArray[4][file] + diffBoardArray[4][file + 1]) / enPassantWeight;
			enPassantBKSide = (diffBoardArray[2][file] + diffBoardArray[3][file] + diffBoardArray[3][file + 1]) / enPassantWeight;

			// Places the values for the kingside into the tilePairs and with the according squares.
			tilePairs.emplace_back(enPassantWKSide, 4, file + 1, 5, file);
			tilePairs.emplace_back(enPassantBKSide, 3, file + 1, 2, file);
		}
		else if (file == 7)
		{
			// Since file 7 (0 indexed) is the H row, the pawn can only be taken from the queenside since the king side is off the board.
			enPassantWQSide = (diffBoardArray[5][file] + diffBoardArray[4][file] + diffBoardArray[4][file - 1]) / enPassantWeight;
			enPassantBQSide = (diffBoardArray[2][file] + diffBoardArray[3][file] + diffBoardArray[3][file - 1]) / enPassantWeight;

			// Places the values for the queenside into the tilePairs and with the according squares.
			tilePairs.emplace_back(enPassantWQSide, 4, file - 1, 5, file);
			tilePairs.emplace_back(enPassantBQSide, 3, file - 1, 2, file);
		}
		else
			// If the file is not 0 or 7, the pawn can be taken from both sides.
		{
			enPassantWKSide = (diffBoardArray[5][file] + diffBoardArray[4][file] + diffBoardArray[4][file + 1]) / enPassantWeight;
			enPassantBKSide = (diffBoardArray[2][file] + diffBoardArray[3][file] + diffBoardArray[3][file + 1]) / enPassantWeight;

			enPassantWQSide = (diffBoardArray[5][file] + diffBoardArray[4][file] + diffBoardArray[4][file - 1]) / enPassantWeight;
			enPassantBQSide = (diffBoardArray[2][file] + diffBoardArray[3][file] + diffBoardArray[3][file - 1]) / enPassantWeight;

			tilePairs.emplace_back(enPassantWKSide, 4, file + 1, 5, file);
			tilePairs.emplace_back(enPassantBKSide, 3, file + 1, 2, file);
			tilePairs.emplace_back(enPassantWQSide, 4, file - 1, 5, file);
			tilePairs.emplace_back(enPassantBQSide, 3, file - 1, 2, file);
		}
	}

	// std::cout << "castleScoreWQ: " << castleScoreWQ << std::endl;
	// std::cout << "castleScoreWK: " << castleScoreWK << std::endl;
	// std::cout << "castleScoreBQ: " << castleScoreBQ << std::endl;
	// std::cout << "castleScoreBK: " << castleScoreBK << std::endl;
	tilePairs.emplace_back(castleScoreWQ, 0, 0, 0, Utill::WQ);
	tilePairs.emplace_back(castleScoreWK, 0, 0, 0, Utill::WK);
	tilePairs.emplace_back(castleScoreBQ, 0, 0, 0, Utill::BQ);
	tilePairs.emplace_back(castleScoreBK, 0, 0, 0, Utill::BK);

	// Sort tile pairs by combined change score in descending order
	std::sort(tilePairs.rbegin(), tilePairs.rend());

	// std::cout << "actualMoveScore = " << std::get<0>(tilePairs[depth]) << std::endl;
	// std::cout << "actualMoveRow1 = " << std::get<1>(tilePairs[depth]) << std::endl;
	// std::cout << "actualMoveCol1 = " << std::get<2>(tilePairs[depth]) << std::endl;
	// std::cout << "actualMoveRow2 = " << std::get<3>(tilePairs[depth]) << std::endl;
	// std::cout << "actualMoveCol2 = " << std::get<4>(tilePairs[depth]) << std::endl;
	// std::cout << "first tile score = " << diffBoardArray[std::get<1>(tilePairs[depth])][std::get<2>(tilePairs[depth])] << std::endl;
	// std::cout << "second tile score = " << diffBoardArray[std::get<3>(tilePairs[depth])][std::get<4>(tilePairs[depth])] << std::endl;

	return std::get<1>(tilePairs[depth]) * 1000 + std::get<2>(tilePairs[depth]) * 100 + std::get<3>(tilePairs[depth]) * 10 + std::get<4>(tilePairs[depth]);
}