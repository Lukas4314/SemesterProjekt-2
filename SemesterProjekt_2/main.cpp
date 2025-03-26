#include "iostream"
#include "string"
#include "AllInOneMain.h"
#include "opencv2/opencv.hpp"
#include <string>
#include "ChessBoard.h"
#include "StockfishUCI.h"

using namespace std;

int main(){
    ChessBoard chess; 
    StockfishUCI engine;

    cout << "FEN is: " << chess.getFEN() <<endl;


    AllInOneMain allInOneMain = AllInOneMain();
    allInOneMain.getPieceMovedString();
    cv::waitKey(0);
    while (true) {

        string move = allInOneMain.getPieceMovedString();
        std::cout << "Camera Move is: " << move << std::endl;
        chess.applyMoveStringCamera(move);
        chess.printBoard();
        

        cout << "Apllying engine move" << endl;
        chess.applyBestMoveFromEngine(engine);
        chess.printBoard();



        //robot moves and makes it move
        cv::waitKey(0);

        allInOneMain.getPieceMovedString();

        if (cv::waitKey(0) == 'q') {
            break;
        }
    }

    return 0;
}