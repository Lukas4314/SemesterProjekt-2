// Include libraries
#include <string>
#include "StockfishUCI.h"
using namespace std;

// ChessBoard class for handling terminal output, player moves, updating FEN, and for applying the best move from stockfish
class ChessBoard
{
public:
    ChessBoard();                                                       // Constructor
    void setupBoard();                                                  // Function for setting up the board
    void updateFEN();                                                   // Function for updating the FEN notation
    string getFEN();                                                    // Function for returning current FEN
    void movePiece(int fromRow, int fromCol, int toRow, int toCol);     // Function for moving pieces
    bool playerMove();                                                  // Function for playing a move
    bool applyMoveStringCamera(const std::string &move);                // Function for applying a move from the camera
    void applyBestMoveFromEngine(StockfishUCI &engine, int depth = 15); // Function for applying the best move from the engine
    void printBoard();                                                  // Function for printing the board
    void checkPieces();
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
};
