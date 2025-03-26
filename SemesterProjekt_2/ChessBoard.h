// Include libraries
#include <iostream>
#include <vector>
#include <string>
#include "StockfishUCI.h"

using namespace std;

// ChessBoard class for handling terminal output, player moves, updating FEN, and for applying the best move from stockfish
class ChessBoard {
private:
    vector<vector<char>> board;     // A vector of vectors with characters
    
    // Variables
    string FEN;
    string activeColor;
    string castlingRights;
    string enPassant;
    int halfmoveClock;
    int fullmoveNumber;

    vector<string> moveHistory;     // A vector with a string which contains the move history

public:
    // Constructer that runs everytime a ChessBoard object is created
    ChessBoard() : board(8, vector<char>(8, '-')) {     // This is how we initialize the board. It takes 8 for 8 vectors, and then we specify that every element of each vector should be empty
        setupBoard();   // Set up the board in the starting position
        // Initiates variables for FEN notation
        activeColor = "w";  
        castlingRights = "KQkq";
        enPassant = "-";
        halfmoveClock = 0;
        fullmoveNumber = 1;
        updateFEN();    // Using the update FEN function it updates the FEN to reflect the boards setup
    }

    // Function for setting up board
    void setupBoard() {
        string pieces = "RNBQKBNR";     // Shortcut string
        for (int i = 0; i < 8; ++i) {   // For loop that iterates from i = 0 to i = 8
            board[0][i] = pieces[i];    // In the first loop R is added to board position 1
            board[1][i] = 'P';          // And a capital P is added to vector 2 position 1
            board[6][i] = 'p';          // Same but for setting up black pieces
            board[7][i] = tolower(pieces[i]);   // Uses tolower function to make back row
        }
    }

    // Function for updating the FEN notation
    void updateFEN() {
        FEN = "";
        for (int i = 7; i >= 0; --i) {
            int emptyCount = 0;
            for (int j = 0; j < 8; ++j) {
                if (board[i][j] == '-') {
                    emptyCount++;
                } else {
                    if (emptyCount > 0) {
                        FEN += to_string(emptyCount);
                        emptyCount = 0;
                    }
                    FEN += board[i][j];
                }
            }
            if (emptyCount > 0) {
                FEN += to_string(emptyCount);
            }
            if (i > 0) FEN += "/";
        }
        FEN += " " + activeColor + " " + castlingRights + " " + enPassant + " " + to_string(halfmoveClock) + " " + to_string(fullmoveNumber);
    }

    // Function for returning current FEN
    string getFEN() const {
        return FEN;
    }

    // Function used to move pieces
    void movePiece(int fromRow, int fromCol, int toRow, int toCol) {
        if (fromRow >= 0 && fromRow < 8 && fromCol >= 0 && fromCol < 8 &&
            toRow >= 0 && toRow < 8 && toCol >= 0 && toCol < 8) {   // If statement that checks that the move is within the frame of the vector of vectors

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
        } else {
            cerr << "Invalid move!" << endl;
        }
    }

    // Function used so that the player can apply moves manually
    bool playerMove() {
        string move;
        cout << "Enter your move (e.g., a2a4) or type 'quit' to exit: ";    // Output to console
        cin >> move;    // Input is the players move which is a string

        if (move == "quit") {   // If statement to exit 
            return false;
        }

        if (move.length() == 4) {   // Checks validity of move by checking length
            int fromCol = move[0] - 'a';
            int fromRow = 8 - (move[1] - '0');
            int toCol = move[2] - 'a';
            int toRow = 8 - (move[3] - '0');

            movePiece(fromRow, fromCol, toRow, toCol);  // Uses move piece function to move piece
        } else {
            cout << "Invalid move format!" << endl;
        }
        return true;
    }

    // This function: 1. Sends all previous moves to stockfish. 2. Asks for the best move (using the stockfishUCI class). 3. Extracts the best move. 4. Applies it to your board
    void applyBestMoveFromEngine(StockfishUCI& engine, int depth = 15) {    // Takes a reference to a StockfishUCI object and a depth
        string allMoves;    // String of all moves 
        for (const string& m : moveHistory) {   // Here we add move to all moves
            allMoves += m + " ";
        }

        string output = engine.getBestMove(allMoves, depth);    // Uses getBestMove function by passing depth and all moves. Stockfish knows to apply all moves to the initial board position and return a bunch of info like "best move b2b4, ponder g4g5" so we store this in output and later we extract the best move

        size_t pos = output.find("bestmove ");  // In the output variable we use the built in find function to search for the keyword bestmove
        // If the pos variable doesnt contain a string with the word bestmove we print an error message
        if (pos == string::npos) {
            cerr << "Failed to find bestmove in engine output!" << endl;
            return;
        }

        string bestMove = output.substr(pos + 9, 4);    // Here we use the substr function to extract the best move and skip past the "bestmove " string and straight to the important part ex. b2b3

        // This is how we convert a2a4 (example) to a move which can be applied by movePiece
        int fromCol = bestMove[0] - 'a';
        int fromRow = 8 - (bestMove[1] - '0');
        int toCol = bestMove[2] - 'a';
        int toRow = 8 - (bestMove[3] - '0');

        movePiece(fromRow, fromCol, toRow, toCol);  // Next we use the movePiece function to move the piece
    }

    // Function which prints the board to the console
    void printBoard() const {
        for (const auto& row : board) {
            for (char cell : row) {
                cout << cell << " ";
            }
            cout << endl;
        }
    }

    // Function for checking amount of pieces on board
    void checkPieces() {
        int count = 0;
        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                if (board[row][col] != '-') {
                    ++count;
                }
            }
        }
        cout << "Number of filled squares: " << count << endl;
    }
    
    
};

