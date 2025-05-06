// Include libraries
#pragma once
#include <string>
#include "StockfishUCI.h"
#include <rclcpp/rclcpp.hpp> // For git logger
#include "ur5_sem2_scripts/moveStruct.hpp"
using namespace std;

// ChessBoard class for handling terminal output, player moves, updating FEN, and for applying the best move from stockfish
class ChessBoard
{
public:
    ChessBoard();                                                       // Constructor
    void setupBoard();
    vector<string> getMoveHistory();                                                  // Function for setting up the board
    void updateFEN();                                                   // Function for updating the FEN notation
    string getFEN();                                                    // Function for returning current FEN
    void movePieceEnPassant(int fromRow, int fromCol, int toRow, int toCol); // Function for moving pieces
    void movePiecePromo(int fromRow, int fromCol, int toRow, int toCol); 
    void movePiece(int fromRow, int fromCol, int toRow, int toCol);     // Function for moving pieces
    bool playerMove();                                                  // Function for playing a move
    bool applyMoveString(string move, StockfishUCI &engine, int depth = 15);                        
    bool applyIfValidMove(string move); // Function for applying a move from the camera
    // void applyBestMoveFromEngine(StockfishUCI &engine, int depth = 15); // Function for applying the best move from the engine
    void printBoard();                                                  // Function for printing the board
    void checkPieces();
    void setBoard(const vector<vector<char>> &newBoard);             // Function for setting the board
    void resetMoveStruct();
    MoveStruct getMoveStruct();
    ~ChessBoard();                                                     // Destructor

private:
    vector<vector<char>> board; // A vector of vectors with characters

    bool bQ = true;
    bool bK = true;
    bool wQ = true;
    bool wK = true;

    // Variables
    string FEN;
    string activeColor;
    string castlingRights;
    string enPassant;
    int halfmoveClock;
    int fullmoveNumber;

    vector<string> moveHistory; // A vector with a string which contains the move history

    MoveStruct moveStruct; // A struct that contains the move information

    rclcpp::Logger const logger = rclcpp::get_logger("Board");
};
