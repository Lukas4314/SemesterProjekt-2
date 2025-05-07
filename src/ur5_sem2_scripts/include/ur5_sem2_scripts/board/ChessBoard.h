#pragma once
#include <string>
#include "StockfishUCI.h"
#include <rclcpp/rclcpp.hpp> // For git logger
#include "ur5_sem2_scripts/moveStruct.hpp"
using namespace std;

class ChessBoard
{
public:
    ChessBoard();                                                       
    void setupBoard();
    void setBoard(const vector<vector<char>> &newBoard);                                                              
    void updateFEN();                                                   
    string getFEN();                                                    
    vector<string> getMoveHistory();                                                 
    static bool isWhite(char piece);
    static bool isBlack(char piece);
    void movePiece(int fromRow, int fromCol, int toRow, int toCol);
    void removePiece(int row, int col);
    void setPiece(int row, int col, char piece);
    void doCastle(string &move);                                                      
    void doSwitch(string &move);
    bool shouldSwitch(char piece1);
    void promoteAllEndRowPawns();                       
    bool applyIfValidMove(string move);
    void printBoard();                                                 
    void resetMoveStruct();
    MoveStruct getMoveStruct();
    ~ChessBoard();                                                   

private:
    vector<vector<char>> board;

    string FEN;
    string activeColor;
    string castlingRights;
    string enPassant;
    int halfmoveClock;
    int fullmoveNumber;

    vector<string> moveHistory; 

    MoveStruct moveStruct;

    rclcpp::Logger const logger = rclcpp::get_logger("Board");
};
