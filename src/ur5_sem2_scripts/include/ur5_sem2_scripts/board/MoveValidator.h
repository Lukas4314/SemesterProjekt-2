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
    static bool isValidMove(int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board, string &activeColor, const vector<string>& moveHistory);
    static bool isValidCastle(string activeColor, bool castleBools[], string castle, const vector<vector<char>> &board, const vector<string>& moveHistory);
    static bool isValidPromotion(int fromRow, int fromCol, int toRow, const vector<vector<char>> &board);
    static bool isValidEnPassant(string &activeColor, const vector<vector<char>> &board, const string &currentMove, const vector<string> &moveHistory);
    ~MoveValidator();
private:
    // Function for checking if pawn move is valid (Need to implement isPathClear function)
    static bool isValidPawn(char piece, int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board, string &activeColor, const vector<string>& moveHistory);
    static bool isValidRook(char piece, int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board);
    static bool isValidKnight(char piece, int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board);
    static bool isValidBishop(char piece, int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board);
    static bool isValidQueen(char piece, int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board);
    static bool isValidKing(char piece, int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board, string &activeColor);
    static bool underAttack(int row, int col, string &activeColor, const vector<vector<char>> &board, const vector<string>& moveHistory);
};

#endif
