#include "iostream"
#include "string"
#include "AllInOneMain.h"
#include "opencv2/opencv.hpp"
#include <string>
#include "ChessBoard.h"
#include "StockfishUCI.h"

using namespace std;

int main(int argc, char *argv[])
{

    
    // Initialize RCLCPP
    rclcpp::init(argc, argv);
    rclcpp::Logger const logger = rclcpp::get_logger("vision_main");
    auto const node = std::make_shared<rclcpp::Node>(
        "vision_main",
        rclcpp::NodeOptions().automatically_declare_parameters_from_overrides(true));

    RCLCPP_INFO(logger, "Starting chess program");
    RCLCPP_DEBUG(logger, "Debugging started");
    int camera_index;
    node->get_parameter("camera_index", camera_index);  // Retrieve the parameter
    ChessBoard chess; 
    StockfishUCI engine;
    int moveCounter = 0;
    cout << "FEN is: " << chess.getFEN() <<endl;

    AllInOneMain allInOneMain = AllInOneMain(camera_index);
    allInOneMain.getPieceMovedString(0);
    
    std::array<std::array<double, 4>, 4> TF = allInOneMain.getBoardCutter(0).getTFchess(TOPLEFTMODE);

    std::cout << "TF is: " << std::endl;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << TF[i][j] << " ";
        }
        std::cout << std::endl;
    }

    cv::waitKey(0);
    while (rclcpp::ok()) {
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

    
    rclcpp::shutdown();
    return 0;
}