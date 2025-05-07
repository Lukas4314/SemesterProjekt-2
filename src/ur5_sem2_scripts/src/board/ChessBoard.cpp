// Include libraries
#include <iostream>
#include <vector>
#include <string>
#include "StockfishUCI.h"
#include "MoveValidator.h"
#include "Utill.h"
#include "ChessBoard.h"
#include <rclcpp/rclcpp.hpp> // For git logger
#include "ur5_sem2_scripts/moveStruct.hpp"
using namespace std;

// Constructer that runs everytime a ChessBoard object is created
ChessBoard::ChessBoard()
{                                                          // This is how we initialize the board. It takes 8 for 8 vectors, and then we specify that every element of each vector should be empty
    board = vector<vector<char>>(8, vector<char>(8, '-')); // A vector of vectors with characters
    setupBoard();                                          // Set up the board in the starting position
    // Initiates variables for FEN notation
    activeColor = "w";
    castlingRights = "KQkq";
    enPassant = "-";

    // FEN THINGS
    halfmoveClock = 0;
    fullmoveNumber = 1;
    updateFEN(); // Using the update FEN function it updates the FEN to reflect the boards setup

    MoveStruct moveStruct;
    resetMoveStruct();
}

ChessBoard::~ChessBoard()
{
}

// Function for setting up board
void ChessBoard::setupBoard()
{
    string pieces = "RNBQKBNR"; // Shortcut string
    for (int i = 0; i < 8; ++i)
    {                                     // For loop that iterates from i = 0 to i = 8
        board[0][i] = pieces[i];          // In the first loop R is added to board position 1
        board[1][i] = 'P';                // And a capital P is added to vector 2 position 1
        board[6][i] = 'p';                // Same but for setting up black pieces
        board[7][i] = tolower(pieces[i]); // Uses tolower function to make back row
    }
}

void ChessBoard::setBoard(const vector<vector<char>> &newBoard)
{
    board = newBoard; // Sets the board to the new board
}

// Function for updating the FEN notation
void ChessBoard::updateFEN()
{
    FEN = "";
    for (int i = 7; i >= 0; --i)
    {
        int emptyCount = 0;
        for (int j = 0; j < 8; ++j)
        {
            if (board[i][j] == '-')
            {
                emptyCount++;
            }
            else
            {
                if (emptyCount > 0)
                {
                    FEN += to_string(emptyCount);
                    emptyCount = 0;
                }
                FEN += board[i][j];
            }
        }
        if (emptyCount > 0)
        {
            FEN += to_string(emptyCount);
        }
        if (i > 0)
            FEN += "/";
    }
    FEN += " " + activeColor + " " + castlingRights + " " + enPassant + " " + to_string(halfmoveClock) + " " + to_string(fullmoveNumber);
}

// Function for returning current FEN
string ChessBoard::getFEN()
{
    return FEN;
}

vector<string> ChessBoard::getMoveHistory()
{
    return moveHistory;
}

bool ChessBoard::isWhite(char piece)
{
    return islower(piece);
}

bool ChessBoard::isBlack(char piece)
{
    return isupper(piece);
}

void ChessBoard::movePiece(int fromRow, int fromCol, int toRow, int toCol)
{
    RCLCPP_DEBUG(logger, "moving piece from %d, %d to %d, %d", fromRow, fromCol, toRow, toCol);

    char piece = board[fromRow][fromCol];
    board[toRow][toCol] = piece;
    board[fromRow][fromCol] = '-';
}

void ChessBoard::removePiece(int row, int col)
{
    RCLCPP_DEBUG(logger, "Removing piece at %d, %d", row, col);
    board[row][col] = '-';
}

void ChessBoard::setPiece(int row, int col, char piece)
{
    RCLCPP_DEBUG(logger, "Setting piece at %d, %d to %c", row, col, piece);
    board[row][col] = piece;
}

void ChessBoard::doCastle(string &move)
{
    if (move == "wQ")
    {
        movePiece(7, 4, 7, 2); // Move the king
        movePiece(7, 0, 7, 3); // Move the rook
        RCLCPP_DEBUG(logger, "Move is : e1c1");
        moveHistory.push_back("e1c1");
    }
    else if (move == "wK")
    {
        movePiece(7, 4, 7, 6); // Move the king
        movePiece(7, 7, 7, 5); // Move the rook
        RCLCPP_DEBUG(logger, "Move is : e1g1");
        moveHistory.push_back("e1g1");
    }

    else if (move == "bQ")
    {
        movePiece(0, 4, 0, 2); // Move the king
        movePiece(0, 0, 0, 3); // Move the rook
        RCLCPP_DEBUG(logger, "Move is : e8c8");
        moveHistory.push_back("e8c8");
    }
    else if (move == "bK")
    {
        movePiece(0, 4, 0, 6); // Move the king
        movePiece(0, 7, 0, 5); // Move the rook
        RCLCPP_DEBUG(logger, "Move is : e8g8");
        moveHistory.push_back("e8g8");
    }
}

void ChessBoard::doSwitch(string &move)
{
    move = move.substr(2, 2) + move.substr(0, 2);
}

bool ChessBoard::shouldSwitch(char piece1)
{
    if (activeColor == "w" && isBlack(piece1))
    {
        return true;
    }

    else if (activeColor == "b" && isWhite(piece1))
    {
        return true;
    }

    else if (piece1 == '-')
    {
        return true;
    }
    return false;
}

// If promotion doesnt work: ISSUE is board[7] should maybe be board[0]
void ChessBoard::promoteAllEndRowPawns() 
{
    for (int col = 0; col < 8; ++col)
    {
        if (board[7][col] == 'P')
        {
            board[7][col] = 'Q';
        }
        else if (board[0][col] == 'p')
        {
            board[0][col] = 'q';
        }
    }
}

// Function for applying move from camere
bool ChessBoard::applyIfValidMove(string move)
{
    RCLCPP_DEBUG(logger, "Entered applyIfValidMove");
    RCLCPP_DEBUG(logger, "Move is: %s", move.c_str());
    RCLCPP_DEBUG(logger, "Active color is: %s", activeColor.c_str());

    if (move == "wQ" || move == "wK" || move == "bQ" || move == "bK")
    {
        RCLCPP_DEBUG(logger, "Tries to castle");
        bool validCastle = MoveValidator::isValidCastle(activeColor, move, board, moveHistory);
        if (!validCastle)
        {
            RCLCPP_DEBUG(logger, "MoveValidator siger nej til castle");
            return false;
        }

        RCLCPP_DEBUG(logger, "It is valid castle");
        doCastle(move);
        activeColor = (activeColor == "w") ? "b" : "w";
        moveHistory.push_back(Utill::translateToEngine(move));
        return true;
    }

    char piece1 = board[8 - (move[1] - '0')][move[0] - 'a'];

    int toCol;
    int toRow;
    int fromCol;
    int fromRow;

    if (shouldSwitch(piece1))
    {
        doSwitch(move);
    }

    piece1 = board[8 - (move[1] - '0')][move[0] - 'a'];

    fromCol = move[0] - 'a';
    fromRow = 8 - (move[1] - '0');
    toCol = move[2] - 'a';
    toRow = 8 - (move[3] - '0');

    // Always checks for an enpassant move, since it is the easiest way to check if the move is a enpassant move is to check if the move
    // Is a valid enpassant move
    bool isValidEnPassant = MoveValidator::isValidEnPassant(activeColor, move, moveHistory);

    if (isValidEnPassant)
    {
        movePiece(fromRow, fromCol, toRow, toCol);
        if (activeColor == "w")
        {
            removePiece(toRow, toCol - 1);
        }
        else
        {
            removePiece(toRow, toCol + 1);
        }
        activeColor = (activeColor == "w") ? "b" : "w";
        moveHistory.push_back(Utill::translateToEngine(move));
        RCLCPP_DEBUG(logger, "The en passant is valid");
        return true;
    }

    bool validMove = MoveValidator::isValidMove(fromRow, fromCol, toRow, toCol, board, activeColor, moveHistory);
    if (!validMove)
    {
        RCLCPP_DEBUG(logger, "MoveValidator returns false");
        return false;
    }

    movePiece(fromRow, fromCol, toRow, toCol);
    activeColor = (activeColor == "w") ? "b" : "w";
    moveHistory.push_back(Utill::translateToEngine(move));

    // Handle all promotions
    promoteAllEndRowPawns();
    return true;
}



// Function which prints the board to the console
void ChessBoard::printBoard()
{
    for (const auto &row : board)
    {
        for (char cell : row)
        {
            cout << cell << " ";
        }
        cout << endl;
    }
}

MoveStruct ChessBoard::getMoveStruct()
{
    return MoveStruct();
}

void ChessBoard::resetMoveStruct()
{
    moveStruct.piece = '-';
    moveStruct.captured = '-';
    moveStruct.color = '-';
    moveStruct.start[0] = -1;
    moveStruct.start[1] = -1;
    moveStruct.end[0] = -1;
    moveStruct.end[1] = -1;
    moveStruct.type = '-';
    moveStruct.promotion = '-';
}