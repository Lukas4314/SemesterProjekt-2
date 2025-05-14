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
    string& getActiveColor();
    void updateHalfMoveClock(string move, vector<vector<char>> boardCopy);
    bool isThreefoldRule();
    void setBoard(const vector<vector<char>> &newBoard);
    vector<vector<char>> getBoard();                                                                                                                 
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
    bool isMated(string &color);
    bool reachedFiftyMoveRule();
    bool isInsufficientMaterial();
    bool isStalemate(string &color);
    bool isRemi(string &color);     
    void updateTurn(string move, vector<vector<char>> boardCopy);                  
    bool applyIfValidMove(string move);
    void printBoard();
    void updateMoveStruct(char piece, char captured, string activeColor, int fromRow, int fromCol, int toRow, int toCol, char type, bool promotion);                                                 
    void resetMoveStruct();
    MoveStruct getMoveStruct();
    ~ChessBoard();                                                   

private:
    vector<vector<char>> board;

    string activeColor;
    string castlingRights;
    string enPassant;
    int halfMoveClock;

    vector<string> moveHistory; 

    vector<vector<vector<char>>> boardHistory; // History of the board states

    MoveStruct moveStruct;

    char promotedTo = '-';

    rclcpp::Logger const logger = rclcpp::get_logger("Board");
};
