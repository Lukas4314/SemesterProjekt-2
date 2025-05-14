#include "MoveValidator.h"
#include "Utill.h"
#include "ChessBoard.h"
#include <iostream>
#include <string>
#include <ChessBoard.h>

using namespace std;

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv); // Initialize the ROS 2 client library
    rclcpp::Logger const logger = rclcpp::get_logger("Test_Validator");
    auto const node = std::make_shared<rclcpp::Node>(
        "Test_Validator",
        rclcpp::NodeOptions().automatically_declare_parameters_from_overrides(true));

    ChessBoard chessBoard = ChessBoard();
    chessBoard.setBoard(
        vector<vector<char>>{
            {'R', '-', '-', '-', 'K', '-', '-', 'R'},
            {'-', '-', '-', '-', '-', '-', '-', '-'},
            {'-', '-', '-', '-', '-', '-', '-', '-'},
            {'-', '-', '-', '-', '-', '-', '-', '-'},
            {'-', '-', '-', '-', '-', '-', '-', '-'},
            {'-', '-', '-', '-', '-', '-', '-', '-'},
            {'-', '-', '-', '-', '-', '-', '-', '-'},
            {'r', '-', '-', 'k', '-', '-', '-', 'r'}});
        

    chessBoard.printBoard();

    vector<vector<char>> boardCopy = chessBoard.getBoard();

    RCLCPP_DEBUG(logger, to_string(chessBoard.applyIfValidMove("d1e1")).c_str());
    chessBoard.printBoard();

    RCLCPP_DEBUG(logger, to_string(chessBoard.applyIfValidMove("e8f8")).c_str());
    chessBoard.printBoard();

    cout << "Remi!" << chessBoard.isRemi(chessBoard.getActiveColor()) << endl;

    RCLCPP_DEBUG(logger, to_string(chessBoard.applyIfValidMove("e1f1")).c_str());
    chessBoard.printBoard();

    RCLCPP_DEBUG(logger, to_string(chessBoard.applyIfValidMove("f8e8")).c_str());
    chessBoard.printBoard();

    cout << "Remi!" << chessBoard.isRemi(chessBoard.getActiveColor()) << endl;

    RCLCPP_DEBUG(logger, to_string(chessBoard.applyIfValidMove("f1e1")).c_str());
    chessBoard.printBoard();

    RCLCPP_DEBUG(logger, to_string(chessBoard.applyIfValidMove("e8f8")).c_str());
    chessBoard.printBoard();

    cout << "Remi!" << chessBoard.isRemi(chessBoard.getActiveColor()) << endl;

    RCLCPP_DEBUG(logger, to_string(chessBoard.applyIfValidMove("e1f1")).c_str());
    chessBoard.printBoard();

    RCLCPP_DEBUG(logger, to_string(chessBoard.applyIfValidMove("f8e8")).c_str());
    chessBoard.printBoard();

    RCLCPP_DEBUG(logger, to_string(chessBoard.applyIfValidMove("f1e1")).c_str());
    chessBoard.printBoard();

    cout << "Remi!" << chessBoard.isRemi(chessBoard.getActiveColor()) << endl;

    return 0;

    // Moves for getting checkmated are: g2g4 d7d5 f2f3 e7e5 f1g2 b8c6 e2e4 d5e4 f3e4 f8c5 b1c3 d8h4 e1e2 h4f2 e2d3 c6b4 d3c4 c8e6 c4b5 a7a6 b5a4 b7b5 a4a3 

}
