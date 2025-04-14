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
    int moveCounter = 0;
    cout << "FEN is: " << chess.getFEN() <<endl;

    AllInOneMain allInOneMain = AllInOneMain();
    allInOneMain.getPieceMovedString(0);

    cv::waitKey(0);
    while (true) {
        string move = allInOneMain.getPieceMovedString(0);
        std::cout << "Move is: " << move << std::endl;

        bool succesMove = chess.applyMoveStringCamera(move);
        std::cout << "Succes move: " << succesMove << std::endl;


        int moveDepth = 0;
        while (!succesMove){
            moveDepth++;
            move = allInOneMain.getPieceMovedString(moveDepth);
            std::cout << "Move is: " << move << std::endl;
            succesMove = chess.applyMoveStringCamera(move);
            std::cout << "Succes move: " << succesMove << std::endl;
        }

        std::cout << "Camera Move is: " << move << std::endl;

        chess.printBoard();
        

        cout << "Apllying engine move" << endl;
        chess.applyBestMoveFromEngine(engine);
        chess.printBoard();



        //robot moves and makes it move
        cv::waitKey(0);

        moveCounter++;
        std::cout << "Move counter: " << moveCounter << std::endl;

        allInOneMain.getPieceMovedString(0);
        std::cout <<"IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII" << std::endl;
        if (cv::waitKey(0) == 'q') {
            break;
        }
    }

    return 0;
}