#include "MoveValidator.h"
#include "Utill.h"
#include "ChessBoard.h"
#include <iostream>
#include <string>
#include <ChessBoard.h>

using namespace std;

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv); // Initialize the ROS 2 client library
    rclcpp::Logger const logger = rclcpp::get_logger("Test_Validator");
    auto const node = std::make_shared<rclcpp::Node>(
        "Test_Validator",
        rclcpp::NodeOptions().automatically_declare_parameters_from_overrides(true));

    ChessBoard chessBoard;
    chessBoard.setBoard(
        // Example setup: You can modify this to set up the board in a specific state
        vector<vector<char>>{
            {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'},
            {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
            {'-', '-', '-', '-', '-', '-', '-', '-'},
            {'-', '-', '-', '-', '-', '-', '-', '-'},
            {'-', '-', '-', '-', '-', '-', '-', '-'},
            {'-', '-', '-', '-', '-', '-', '-', '-'},
            {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
            {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'}
        }
    );

    chessBoard.printBoard();

    RCLCPP_DEBUG(logger, to_string(chessBoard.applyIfValidMove("f2f4")).c_str()); 
    chessBoard.printBoard();

    RCLCPP_DEBUG(logger, to_string(chessBoard.applyIfValidMove("d7d5")).c_str()); 
    chessBoard.printBoard();

    RCLCPP_DEBUG(logger, to_string(chessBoard.applyIfValidMove("g1f3")).c_str()); 
    chessBoard.printBoard();

    RCLCPP_DEBUG(logger, to_string(chessBoard.applyIfValidMove("c7c5")).c_str()); 
    chessBoard.printBoard();
    
    RCLCPP_DEBUG(logger, to_string(chessBoard.applyIfValidMove("g2g4")).c_str()); 
    chessBoard.printBoard();

    RCLCPP_DEBUG(logger, to_string(chessBoard.applyIfValidMove("c8g4")).c_str()); 
    chessBoard.printBoard();

    RCLCPP_DEBUG(logger, to_string(chessBoard.applyIfValidMove("f1g2")).c_str()); 
    chessBoard.printBoard();

    RCLCPP_DEBUG(logger, to_string(chessBoard.applyIfValidMove("e7e6")).c_str()); 
    chessBoard.printBoard();

    RCLCPP_DEBUG(logger, to_string(chessBoard.applyIfValidMove("wK")).c_str()); 
    chessBoard.printBoard();
    
    return 0;   
}
