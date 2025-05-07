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
            {'R', '-', 'B', '-', 'K', 'B', '-', 'R'},
            {'-', 'P', '-', '-', '-', 'P', 'P', 'P'},
            {'-', '-', '-', '-', '-', 'Q', '-', '-'},
            {'-', '-', '-', '-', 'P', '-', '-', '-'},
            {'-', '-', 'p', '-', '-', '-', 'p', '-'},
            {'-', 'N', '-', 'p', 'p', '-', '-', '-'},
            {'-', '-', '-', '-', '-', 'p', '-', 'p'},
            {'-', '-', 'k', 'r', '-', 'b', 'n', 'r'}});

    chessBoard.printBoard();

    RCLCPP_DEBUG(logger, to_string(chessBoard.applyIfValidMove("c1c2")).c_str());
    chessBoard.printBoard();

    return 0;
}
