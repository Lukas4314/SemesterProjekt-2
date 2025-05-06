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

    RCLCPP_DEBUG(logger, to_string(chessBoard.applyIfValidMove("e2e4")).c_str()); 
    chessBoard.printBoard();

    RCLCPP_DEBUG(logger, to_string(chessBoard.applyIfValidMove("a7a5")).c_str()); 
    chessBoard.printBoard();

    RCLCPP_DEBUG(logger, to_string(chessBoard.applyIfValidMove("d1h5")).c_str()); 
    chessBoard.printBoard();

    RCLCPP_DEBUG(logger, to_string(chessBoard.applyIfValidMove("a5a4")).c_str()); 
    chessBoard.printBoard();

    RCLCPP_DEBUG(logger, to_string(chessBoard.applyIfValidMove("f1c4")).c_str()); 
    chessBoard.printBoard();

    RCLCPP_DEBUG(logger, to_string(chessBoard.applyIfValidMove("a4a3")).c_str()); 
    chessBoard.printBoard();

    RCLCPP_DEBUG(logger, to_string(chessBoard.applyIfValidMove("h5f7")).c_str()); 
    chessBoard.printBoard();

    RCLCPP_DEBUG(logger, to_string(chessBoard.applyIfValidMove("h7h5")).c_str()); 
    chessBoard.printBoard();
    
    return 0;   
}
