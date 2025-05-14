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
    ~MoveValidator();
    static bool correctColor(char piece, string &activeColor);
    static bool isValidMove(int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board, string &activeColor,  bool calledByUnderAttack, bool calledByKing = false);
    static vector<int> findKing(string color, const vector<vector<char>> &board);
    static bool checkHistoryForKingOrRookMovement(const string &move, const vector<string> &moveHistory);
    static bool castlePathUnderAttack(const string &move, const vector<vector<char>> &board);
    static bool emptySquaresForCastle(const string &move, const vector<vector<char>> &board);
    static bool isValidCastle(string activeColor, string move, const vector<vector<char>> &board, const vector<string>& moveHistory);
    static bool isValidEnPassant(string &activeColor, const string &currentMove, const vector<string> &moveHistory);
    static bool underAttack(int row, int col, const string &activeColor, const vector<vector<char>> &board, bool calledByKing = false);
    
private:
    // Function for checking if pawn move is valid (Need to implement isPathClear function)
    static bool isValidPawn(int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board, string &activeColor, bool calledByUnderAttack);
    static bool pathClearBetweenOrthogonal(int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board);
    static bool pathClearBetweenDiagonal(int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board);
    static bool isValidRook(char piece, int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board);
    static bool isValidKnight(char piece, int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board);
    static bool isValidBishop(char piece, int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board);
    static bool isValidQueen(char piece, int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board);
    static bool isValidKing(char piece, int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board, string &activeColor, bool calledByKing = false);
};

#endif
