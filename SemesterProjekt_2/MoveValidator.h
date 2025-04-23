#ifndef MOVEVALIDATOR_H
#define MOVEVALIDATOR_H

#include <string>
#include <vector>
#include <rclcpp/rclcpp.hpp>

using namespace std;

class MoveValidator
{
public:
    // Entry point: validates any move
    MoveValidator();
    static bool isValidMove(int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board, const string &activeColor);
    static bool isValidCastle(string activeColor, bool castleBools[], string castle, const vector<vector<char>> &board);
    static bool isValidPromotion(int fromRow, int fromCol, int toCol, const vector<vector<char>> &board);
    //static bool isValidEnPassant(string lastMove, int fromRow, int fromCol, string activeColor, int toRow, int toCol, const vector<vector<char>> &board);
    ~MoveValidator();
private:
    // Function for checking if pawn move is valid (Need to implement isPathClear function)
    static bool isValidPawn(char piece, int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board, const string &activeColor);
    static bool isValidRook(char piece, int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board);
    static bool isValidKnight(char piece, int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board);
    static bool isValidBishop(char piece, int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board);
    static bool isValidQueen(char piece, int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board);
    static bool isValidKing(char piece, int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board, const string &activeColor);
    static bool underAttack(int row, int col, const string &activeColor, const vector<vector<char>> &board);
    auto const logger = rclcpp::get_logger("Move_Validator")
};

#endif