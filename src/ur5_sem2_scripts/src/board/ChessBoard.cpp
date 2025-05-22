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
#include "ur5_sem2_scripts/logger/Logger.h"
using namespace std;

// Constructer that runs everytime a ChessBoard object is created
ChessBoard::ChessBoard()
{                                                          // This is how we initialize the board. It takes 8 for 8 vectors, and then we specify that every element of each vector should be empty
    board = vector<vector<char>>(8, vector<char>(8, '-')); // A vector of vectors with characters
    setupBoard();                                          // Set up the board in the starting position
    activeColor = "w";
    castlingRights = "KQkq";
    enPassant = "-";
    halfMoveClock = 0;
    moveCount = 0;
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

string &ChessBoard::getActiveColor()
{
    return activeColor;
}

void ChessBoard::updateHalfMoveClock(string move, vector<vector<char>> boardCopy)
{
    if (move == "wK" || move == "wQ" || move == "bK" || move == "bQ")
    {
        halfMoveClock = 0; // Reset the half-move clock if a castle is made
        return;
    }

    int fromCol = move[0] - 'a';
    int fromRow = 8 - (move[1] - '0');
    int toCol = move[2] - 'a';
    int toRow = 8 - (move[3] - '0');

    if (boardCopy[fromRow][fromCol] == 'P' || boardCopy[fromRow][fromCol] == 'p')
    {
        halfMoveClock = 0; // Reset the half-move clock if a pawn move is made
    }
    else if (boardCopy[toRow][toCol] != '-')
    {
        halfMoveClock = 0; // Reset the half-move clock if a capture is made
    }
    else
    {
        halfMoveClock++; // Increment the half-move clock for other moves
    }
}

bool ChessBoard::isThreefoldRule()
{
    int count = 0;

    for (size_t i = 0; i < boardHistory.size(); i++)
    {
        if (boardHistory[i] == board)
        {

            count++;
        }
    }

    cout << "Counter for isThreeFoldRule is: " << count << endl;

    return count >= 3;
}

void ChessBoard::setBoard(const vector<vector<char>> &newBoard)
{
    board = newBoard; // Sets the board to the new board
}

vector<vector<char>> ChessBoard::getBoard()
{
    return board;
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
        moveStruct.start[0] = 4;
        moveStruct.start[1] = 7;
        moveStruct.end[0] = 2;
        moveStruct.end[1] = 7;
        moveStruct.type = 'c';
    }
    else if (move == "wK")
    {
        movePiece(7, 4, 7, 6); // Move the king
        movePiece(7, 7, 7, 5); // Move the rook
        RCLCPP_DEBUG(logger, "Move is : e1g1");
        moveStruct.start[0] = 7;
        moveStruct.start[1] = 4;
        moveStruct.end[0] = 7;
        moveStruct.end[1] = 6;
        moveStruct.type = 'c';
    }

    else if (move == "bQ")
    {
        movePiece(0, 4, 0, 2); // Move the king
        movePiece(0, 0, 0, 3); // Move the rook
        RCLCPP_DEBUG(logger, "Move is : e8c8");
        moveStruct.start[0] = 0;
        moveStruct.start[1] = 4;
        moveStruct.end[0] = 0;
        moveStruct.end[1] = 2;
        moveStruct.type = 'c';
    }
    else if (move == "bK")
    {
        movePiece(0, 4, 0, 6); // Move the king
        movePiece(0, 7, 0, 5); // Move the rook
        RCLCPP_DEBUG(logger, "Move is : e8g8");
        moveStruct.start[0] = 0;
        moveStruct.start[1] = 4;
        moveStruct.end[0] = 0;
        moveStruct.end[1] = 6;
        moveStruct.type = 'c';
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
            if (promotedTo == '-')
            {
                board[7][col] = 'Q';
                moveStruct.type = 'p';
                moveStruct.promotion = 'q';
            }
            else
            {
                board[7][col] = toupper(promotedTo);
                moveStruct.type = 'p';
                moveStruct.promotion = 'q';
            }
        }
        else if (board[0][col] == 'p')
        {
            if (promotedTo == '-')
            {
                board[0][col] = 'q';
                moveStruct.type = 'p';
                moveStruct.promotion = 'q';
            }
            else
            {
                board[0][col] = tolower(promotedTo);
                moveStruct.type = 'p';
                moveStruct.promotion = 'q';
            }
        }
    }
}

bool ChessBoard::isMated(string &color)
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            char piece = board[i][j];
            if (piece != '-' && ((color == "w" && isWhite(piece)) || (color == "b" && isBlack(piece))))
            {
                cout << "Piece: " << piece << endl;
                // Check if the piece can make any valid moves
                for (int toRow = 0; toRow < 8; ++toRow)
                {
                    for (int toCol = 0; toCol < 8; ++toCol)
                    {
                        if (MoveValidator::isValidMove(i, j, toRow, toCol, board, color, false, false))
                        {
                            return false; // Found a valid move, not mated
                        }
                    }
                }
            }
        }
    }
    return true;
}

bool ChessBoard::reachedFiftyMoveRule()
{
    // Check if 50 moves have been made without a pawn move or capture
    if (halfMoveClock >= 50)
    {
        return true; // Fifty-move rule reached
    }
    return false;
}

bool ChessBoard::isInsufficientMaterial()
{
    // Check for insufficient material
    int whitePieces = 0;
    int blackPieces = 0;
    int whiteBishopCount = 0;
    int blackBishopCount = 0;
    int whiteKnightCount = 0;
    int blackKnightCount = 0;

    // - King vs King
    // - King vs King + Bishop
    // - King vs King + Knight
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            char piece = board[i][j];
            if (piece != '-')
            {
                if (isWhite(piece))
                {
                    whitePieces++;
                }
                else
                {
                    blackPieces++;
                }
            }
            if (piece == 'b')
            {
                whiteBishopCount++;
            }
            if (piece == 'B')
            {
                blackBishopCount++;
            }
            if (piece == 'n')
            {
                whiteKnightCount++;
            }
            if (piece == 'N')
            {
                blackKnightCount++;
            }
        }
    }
    if (whitePieces == 1 && blackPieces == 1)
    {
        return true; // Only kings left
    }

    if (whitePieces == 2 && blackPieces == 1 && (whiteBishopCount == 1 || whiteKnightCount == 1))
    {
        return true; // (white)King vs (black)King + Bishop or Knight
    }
    if (blackPieces == 2 && whitePieces == 1 && (blackBishopCount == 1 || blackKnightCount == 1))
    {
        return true; // (black)King vs (white)King + Bishop or Knight
    }
    return false;
}

bool ChessBoard::isStalemate(string &activeColor)
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            char piece = board[i][j];
            if (piece != '-' && ((activeColor == "w" && isWhite(piece)) || (activeColor == "b" && isBlack(piece))))
            {
                // Check if the piece can make any valid moves
                for (int toRow = 0; toRow < 8; ++toRow)
                {
                    for (int toCol = 0; toCol < 8; ++toCol)
                    {
                        if (MoveValidator::isValidMove(i, j, toRow, toCol, board, activeColor, false))
                        {
                            return false; // Found a valid move, not mated
                        }
                    }
                }
            }
        }
    }
    vector<int> kingPos = MoveValidator::findKing(activeColor, board);

    if (MoveValidator::underAttack(kingPos[0], kingPos[1], activeColor, board))
    {
        cout << "Calling all soldiers" << endl;
        return false;
    }

    return true;
}

// Active color is the color which is next to make the move, which means the color which is in stalemate for example
bool ChessBoard::isRemi(string &activeColor)
{
    if (isStalemate(activeColor))
    {
        cout << "Stalemate" << endl;
        return true;
    }

    // Check for stalemate && Check for 50 move rule (no pawn moves, no captures) && Check for insufficient material
    if (reachedFiftyMoveRule())
    {
        cout << "Fifty move rule" << endl;
        return true;
    }

    if (isInsufficientMaterial())
    {
        cout << "Insufficient material" << endl;
        return true;
    }

    if (isThreefoldRule())
    {
        cout << "Threefold rule" << endl;
        return true;
    }
    return false;
}

void ChessBoard::updateTurn(string move, vector<vector<char>> originalBoard)
{
    Logger::setValue(MOVE_COUNT, to_string(moveCount));
    Logger::setValue(MOVE, move);

    // Puts the color (as a char) into the moveStrut before updating the color
    moveStruct.color = activeColor[0];
    activeColor = (activeColor == "w") ? "b" : "w";

    if (promotedTo != '-')
    {
        move += promotedTo;
    }
    moveHistory.push_back(Utill::translateToEngine(move));

    updateHalfMoveClock(move, originalBoard);
    cout << "HalfMoveclock: " << halfMoveClock << endl;
    boardHistory.push_back(board);
}

// Function for applying move from camera
bool ChessBoard::applyIfValidMove(string move)
{
    resetMoveStruct();
    vector<vector<char>> originalBoard = board;

    RCLCPP_DEBUG(logger, "Entered applyIfValidMove");
    RCLCPP_DEBUG(logger, "Move is: %s", move.c_str());
    RCLCPP_DEBUG(logger, "Active color is: %s", activeColor.c_str());

    if (move.length() == 5)
    {
        promotedTo = move.substr(4, 1)[0];
        move = move.substr(0, 4);
    }

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
        updateTurn(move, originalBoard);
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
    char piece2 = board[8 - (move[3] - '0')][move[2] - 'a'];
    fromCol = move[0] - 'a';
    fromRow = 8 - (move[1] - '0');
    toCol = move[2] - 'a';
    toRow = 8 - (move[3] - '0');

    moveStruct.piece = piece1;
    moveStruct.captured = piece2;
    moveStruct.start[0] = fromRow;
    moveStruct.start[1] = fromCol;
    moveStruct.end[0] = toRow;
    moveStruct.end[1] = toCol;

    // Always checks for an enpassant move, since it is the easiest way to check if the move is a enpassant move is to check if the move
    // Is a valid enpassant move
    bool isValidEnPassant = MoveValidator::isValidEnPassant(activeColor, move, moveHistory);

    if (isValidEnPassant)
    {
        movePiece(fromRow, fromCol, toRow, toCol);
        if (activeColor == "w")
        {
            removePiece(toRow + 1, toCol);
        }
        else
        {
            removePiece(toRow - 1, toCol);
        }
        moveStruct.captured = 'p';
        moveStruct.type = 'e';

        updateTurn(move, originalBoard);
        RCLCPP_DEBUG(logger, "The en passant is valid");
        return true;
    }

    bool validMove = MoveValidator::isValidMove(fromRow, fromCol, toRow, toCol, board, activeColor, false);
    if (!validMove)
    {
        RCLCPP_DEBUG(logger, "MoveValidator returns false");
        return false;
    }

    movePiece(fromRow, fromCol, toRow, toCol);
    updateTurn(move, originalBoard);

    moveStruct.type = 'm';

    // Handle all promotions
    promoteAllEndRowPawns();
    promotedTo = '-';
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

void ChessBoard::updateMoveStruct(char piece, char captured, string activeColor, int fromRow, int fromCol, int toRow, int toCol, char type, bool promotion)
{
    moveStruct.piece = piece;
    moveStruct.captured = captured;
    moveStruct.color = (activeColor == "w") ? 'b' : 'w'; // because we need last moves active color, and not next moves.
    moveStruct.start[0] = fromRow;
    moveStruct.start[1] = fromCol;
    moveStruct.end[0] = toRow;
    moveStruct.end[1] = toCol;
    moveStruct.type = type;
    moveStruct.promotion = (promotion) ? 'q' : '-';
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

MoveStruct ChessBoard::getMoveStruct()
{
    // Flips the rows
    moveStruct.start[0] = 7 - moveStruct.start[0];
    moveStruct.end[0] = 7 - moveStruct.end[0];
    return moveStruct;
}