#include "HueDifferenceProcessor.h"
#include "ImageFinder.h"
#include <iostream>
#include <windows.h>
#include "StockfishEngine.h"

int main() {
	std::string enginePath = "stockfish-windows-x86-64-avx2"; // path

	StockfishEngine stockfish(enginePath);

	if (!stockfish.start()) {
		std::cerr << "Failed to start Stockfish!" << std::endl;
		return 1;
	}

	std::string response = stockfish.sendCommand("uci");
	std::cout << response << std::endl;

	response = stockfish.sendCommand("position startpos");
	response = stockfish.sendCommand("go depth 15");
	std::cout << response << std::endl;

	return 0;

	/*
	cv::Mat cheesWithMarkedCornors = cv::imread("chessWithMarkedCornors.jpg", cv::IMREAD_COLOR);
	cv::Mat greenCircle = cv::imread("greenCircle.png", cv::IMREAD_COLOR);
	ImageFinder imageFinder;
	int x = 0;
	int y = 0;
	//imageFinder.findImageInImage(greenCircle, cheesWithMarkedCornors,x, y);
	//std::cout << "x: " << x << " y: " << y << std::endl;
	imageFinder.testStart();
	cv::waitKey(0);
	*/
	
}