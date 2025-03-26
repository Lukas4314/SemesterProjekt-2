#include "iostream"
#include "string"
#include "AllInOneMain.h"
#include "opencv2/opencv.hpp"

int main(){
    
    AllInOneMain allInOneMain = AllInOneMain();

    while (true) {

        allInOneMain.getPieceMovedString();
        std::cout << allInOneMain.getPieceMovedString() << std::endl;

        if (cv::waitKey(0) == 'q') {
            break;
        }
    }

    return 0;
}