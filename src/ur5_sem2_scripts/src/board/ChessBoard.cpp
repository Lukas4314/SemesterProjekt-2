// Include libraries
#include <iostream>
#include <vector>
#include <string>
#include "StockfishUCI.h"
#include "MoveValidator.h"
#include "Utill.h"
#include "ChessBoard.h"
#include <rclcpp/rclcpp.hpp> // For git logger
using namespace std;

// Constructer that runs everytime a ChessBoard object is created
ChessBoard::ChessBoard()
{                                                          // This is how we initialize the board. It takes 8 for 8 vectors, and then we specify that every element of each vector should be empty
    board = vector<vector<char>>(8, vector<char>(8, '-')); // A vector of vectors with characters
    setupBoard();                                          // Set up the board in the starting position
    //setupTestBoard();                                      // Set up the board in the test position
    // Initiates variables for FEN notation
    activeColor = "w";
    castlingRights = "KQkq";
    enPassant = "-";
    halfmoveClock = 0;
    fullmoveNumber = 1;
    updateFEN(); // Using the update FEN function it updates the FEN to reflect the boards setup
}

ChessBoard::~ChessBoard()
{
    // Destructor
    
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

/*
void ChessBoard::setupTestBoard()
{
    string pieces = "RNBQKBNR"; // Shortcut string
    for (int i = 0; i < 8; ++i)
    {                                     // For loop that iterates from i = 0 to i = 8
        board[0][i] = pieces[i];          // In the first loop R is added to board position 1
        board[4][0] = 'P';
        board[1][1] = 'P';
        board[4][2] = 'P';
        board[1][3] = 'P';
        board[4][4] = 'P';
        board[1][5] = 'P';
        board[4][6] = 'P';
        board[1][7] = 'P';                
        board[6][i] = 'p';                // Same but for setting up black pieces
        board[7][i] = tolower(pieces[i]); // Uses tolower function to make back row
    }
}
*/

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

// Function used to move pieces
void ChessBoard::movePiece(int fromRow, int fromCol, int toRow, int toCol)
{
    if (fromRow >= 0 && fromRow < 8 && fromCol >= 0 && fromCol < 8 &&
        toRow >= 0 && toRow < 8 && toCol >= 0 && toCol < 8)
    { // If statement that checks that the move is within the frame of the vector of vectors

        char piece = board[fromRow][fromCol];
        board[toRow][toCol] = piece;
        board[fromRow][fromCol] = '-';

        string move = "";
        move += ('a' + fromCol);
        move += ('8' - fromRow);
        move += ('a' + toCol);
        move += ('8' - toRow);
        moveHistory.push_back(move);

        activeColor = (activeColor == "w") ? "b" : "w";
        fullmoveNumber += (activeColor == "w") ? 1 : 0;
        updateFEN();

        if (piece == 'K')
        {
            bQ = false;
            bK = false;
        }
        else if (piece == 'k')
        {
            wQ = false;
            wK = false;
        }

        else if ((fromRow == 0 && fromCol == 0) || toRow == 0 && toCol == 0)
        {
            bQ = false;
        }
        else if ((fromRow == 0 && fromCol == 7) || toRow == 0 && toCol == 7)
        {
            bK = false;
        }
        else if ((fromRow == 7 && fromCol == 0) || toRow == 7 && toCol == 0)
        {
            wQ = false;
        }
        else if ((fromRow == 7 && fromCol == 7) || toRow == 7 && toCol == 7)
        {
            wK = false;
        }
    }
    else
    {
        cerr << "Invalid move!" << endl;
    }
}

// Function used so that the player can apply moves manually
bool ChessBoard::playerMove()
{
    string move;
    RCLCPP_INFO(logger, "Enter your move (e.g., a2a4) or type 'quit' to exit: ");
    cin >> move;                                                     // Input is the players move which is a string

    if (move == "quit")
    { // If statement to exit
        return false;
    }

    if (move.length() == 4)
    { // Checks validity of move by checking length
        int fromCol = move[0] - 'a';
        int fromRow = 8 - (move[1] - '0');
        int toCol = move[2] - 'a';
        int toRow = 8 - (move[3] - '0');

        movePiece(fromRow, fromCol, toRow, toCol); // Uses move piece function to move piece
    }
    else
    {
        cout << "Invalid move format!" << endl;
        std::cout << "move = " << move << std::endl;
    }
    return true;
}

// Function for applying move from camere
bool ChessBoard::applyMoveStringCamera(const string &move)
{

    if (move == "wQ" || move == "wK" || (move == "bQ" && activeColor != "w") || (move == "bK" && activeColor != "w"))
    {
        bool castleBools[4];
        castleBools[Utill::WQcastleIndex] = wQ;
        castleBools[Utill::WKcastleIndex] = wK;
        castleBools[Utill::BQcastleIndex] = bQ;
        castleBools[Utill::BKcastleIndex] = bK;
        bool validCastle = MoveValidator::isValidCastle(activeColor, castleBools, move, board);
        RCLCPP_DEBUG(logger, "validCastle");
        if (!validCastle)
        {
            RCLCPP_DEBUG(logger, "MoveValidator siger nej til castle");
            return false;
        }

        if (move == "wQ")
        {
            movePiece(7, 4, 7, 2); // Move the king
            movePiece(7, 0, 7, 3); // Move the rook
            wK = false;
            wQ = false;
        }
        else if (move == "wK")
        {
            movePiece(7, 4, 7, 6); // Move the king
            movePiece(7, 7, 7, 5); // Move the rook
            wK = false;
            wQ = false;
        }

        if (move == "bQ")
        {
            movePiece(0, 4, 0, 2); // Move the king
            movePiece(0, 0, 0, 3); // Move the rook
            bK = false;
            bQ = false;
        }
        else if (move == "bK")
        {
            movePiece(0, 4, 0, 6); // Move the king
            movePiece(0, 7, 0, 5); // Move the rook
            bK = false;
            bQ = false;
        }
        activeColor = (activeColor == "w") ? "b" : "w";
        return true;
    }

    char piece1 = board[8 - (move[1] - '0')][move[0] - 'a'];
    char piece2 = board[8 - (move[3] - '0')][move[2] - 'a'];

    int toCol;
    int toRow;
    int fromCol;
    int fromRow;

    if (piece1 == '-')
    {
        toCol = move[0] - 'a';
        toRow = 8 - (move[1] - '0');
        fromCol = move[2] - 'a';
        fromRow = 8 - (move[3] - '0');
        RCLCPP_DEBUG(logger, "Thought it was moving a blank piece and does the switch");
    }

    else if (activeColor == "w" && islower(piece1))
    {
        fromCol = move[0] - 'a';
        fromRow = 8 - (move[1] - '0');
        toCol = move[2] - 'a';
        toRow = 8 - (move[3] - '0');
        RCLCPP_DEBUG(logger, "White moves on its turn but does not do the switch");
    }

    else if (activeColor == "w" && isupper(piece1))
    {
        toCol = move[0] - 'a';
        toRow = 8 - (move[1] - '0');
        fromCol = move[2] - 'a';
        fromRow = 8 - (move[3] - '0');
        RCLCPP_DEBUG(logger, "Thought it was moving a black piece and does the switch");
    }

    else
    {
        fromCol = move[0] - 'a';
        fromRow = 8 - (move[1] - '0');
        toCol = move[2] - 'a';
        toRow = 8 - (move[3] - '0');
        RCLCPP_DEBUG(logger, "This does not do the switch");
    }

    /*
    if (!MoveValidator::isValidEnPassant(lastMove, fromRow, fromCol, activeColor, toRow, toCol, board))
    {// Uses the isValidEnPassant to check if the move is valid
        cout << "Is valid enpassant siger nej" << endl;
        return false;
    }
    */

    bool validMove = MoveValidator::isValidMove(fromRow, fromCol, toRow, toCol, board, activeColor, moveHistory);
    if (!validMove)
    { // Uses the MoveValidator class to check if the move is valid
        RCLCPP_DEBUG(logger, "MoveValidator siger nej");
        return false;
    }

    movePiece(fromRow, fromCol, toRow, toCol);
    return true;
}

// This function: 1. Sends all previous moves to stockfish. 2. Asks for the best move (using the stockfishUCI class). 3. Extracts the best move. 4. Applies it to your board
void ChessBoard::applyBestMoveFromEngine(StockfishUCI &engine, int depth)
{                    // Takes a reference to a StockfishUCI object and a depth
    string allMoves; // String of all moves
    for (const string &m : moveHistory)
    { // Here we add move to all moves
        allMoves += m + " ";
    }

    string output = engine.getBestMove(allMoves, depth); // Uses getBestMove function by passing depth and all moves. Stockfish knows to apply all moves to the initial board position and return a bunch of info like "best move b2b4, ponder g4g5" so we store this in output and later we extract the best move

    size_t pos = output.find("bestmove "); // In the output variable we use the built in find function to search for the keyword bestmove
    // If the pos variable doesnt contain a string with the word bestmove we print an error message
    if (pos == string::npos)
    {
        cerr << "Failed to find bestmove in engine output!" << endl;
        return;
    }

    string bestMove = output.substr(pos + 9, 4); // Here we use the substr function to extract the best move and skip past the "bestmove " string and straight to the important part ex. b2b3
    RCLCPP_DEBUG(logger, ("Bestmove: " + bestMove).c_str());

    // This is how we convert a2a4 (example) to a move which can be applied by movePiece
    int fromCol = bestMove[0] - 'a';
    int fromRow = 8 - (bestMove[1] - '0');
    int toCol = bestMove[2] - 'a';
    int toRow = 8 - (bestMove[3] - '0');

    movePiece(fromRow, fromCol, toRow, toCol); // Next we use the movePiece function to move the piece
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

// Function for checking amount of pieces on board
void ChessBoard::checkPieces()
{
    int count = 0;
    for (int row = 0; row < 8; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            if (board[row][col] != '-')
            {
                ++count;
            }
        }
    }
    RCLCPP_DEBUG(logger, "Number of filled squares: ");
}