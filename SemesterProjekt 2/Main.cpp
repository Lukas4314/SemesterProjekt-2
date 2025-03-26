#define NOMINMAX
#include "HueDifferenceProcessor.h"
#include "ImageFinder.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <algorithm>
#include "boardCutter.h"
#include "ImageDrawer.h"
#include "MoveFinder.h"
#include "AllInOneMain.h"


int main() {
	AllInOneMain allInOneMain = AllInOneMain();
	while (true) {
		std::cout<<allInOneMain.getPieceMovedString()<<std::endl;
		if (cv::waitKey(0) == 'q') {
			break;
		}
	}
}