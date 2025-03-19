#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>
#include "StockFishAPI.h" // Assuming StockfishAPI encapsulates Stockfish interaction

using namespace std;

// Initiates class
class ChessBoard { 
private:
    vector<vector<char>> board; // The board is a 2D vector of characters
    string FEN; // Stores the FEN representation of the board
    string activeColor; // 'w' for white to move, 'b' for black to move
    string castlingRights; // Castling availability
    string enPassant; // En passant target square
    int halfmoveClock; // Halfmove clock for the 50-move rule
    int fullmoveNumber; // Fullmove number

public:
    // Constructor initializes the chessboard with the starting position
    ChessBoard() : board(8, vector<char>(8, '-')) {
        setupBoard(); // Calls setupBoard function to set initial position
        activeColor = "w"; // White starts
        castlingRights = "KQkq"; // All castling rights available at the start
        enPassant = "-"; // No en passant available at start
        halfmoveClock = 0; // Reset halfmove clock
        fullmoveNumber = 1; // Game starts at move 1
        updateFEN(); // Updates the FEN notation
    }
    
    // Function to set up the initial position of a chess game
    void setupBoard() {
        string pieces = "RNBQKBNR"; // This string holds the order of major chess pieces *const ensure that the string is not changed which may be a potential issue
        for (int i = 0; i < 8; ++i) { // For loop that iterates over every row of the chess board
            board[0][i] = pieces[i];  // Calls the board and adds the i major piece to the 2D vector
            board[1][i] = 'P';        // White pawns
            board[6][i] = 'p';        // Black pawns
            board[7][i] = tolower(pieces[i]); // Black major pieces
        }
    }
    
    // Function to update FEN notation based on the current board state
    void updateFEN() {
        FEN = ""; // Reset FEN string
        for (int i = 7; i >= 0; --i) { // Iterate over each row in reverse order
            int emptyCount = 0; // Counter for empty squares
            for (int j = 0; j < 8; ++j) { // Iterate over each column
                if (board[i][j] == '-') { // If the square is empty
                    emptyCount++; // Increase empty square count
                } else { // If a piece is present
                    if (emptyCount > 0) {
                        FEN += to_string(emptyCount); // Append empty squares count to FEN
                        emptyCount = 0;
                    }
                    FEN += board[i][j]; // Append the piece to FEN
                }
            }
            if (emptyCount > 0) {
                FEN += to_string(emptyCount); // Append remaining empty squares count
            }
            if (i > 0) {
                FEN += "/"; // Separate rows with '/'
            }
        }
        
        // Append additional FEN information
        FEN += " " + activeColor + " " + castlingRights + " " + enPassant + " " + to_string(halfmoveClock) + " " + to_string(fullmoveNumber);
    }
    
    // Function to get the FEN notation
    string getFEN() const {
        return FEN; // Return the FEN string
    }
    
    // Function to move a piece on the board takes 4 integers
    void movePiece(int fromRow, int fromCol, int toRow, int toCol) {
        if (fromRow >= 0 && fromRow < 8 && fromCol >= 0 && fromCol < 8 &&
            toRow >= 0 && toRow < 8 && toCol >= 0 && toCol < 8) { // Error checking
            board[toRow][toCol] = board[fromRow][fromCol]; // Move piece
            board[fromRow][fromCol] = '-'; // Empty old position
            activeColor = (activeColor == "w") ? "b" : "w"; // Toggle turn
            fullmoveNumber += (activeColor == "w") ? 1 : 0; // Increase move count only after black moves
            updateFEN(); // Update FEN after move
        } else {
            cerr << "Invalid move!" << endl;
        }
    }
    
    // Function to read best move from Stockfish JSON file and apply it
    void applyBestMoveFromJson(const string& filePath) { // Takes a file path to the json file
        ifstream file(filePath); // Opens the json file
        if (!file.is_open()) {  // For printing error if file was unsuccessfully opened
            cerr << "Failed to open JSON file!" << endl;
            return;
        }
        
        nlohmann::json jsonData;
        file >> jsonData;
        file.close();
        
        string bestMove = jsonData["bestmove"].get<string>(); // Extracts the best move
        if (bestMove.find("bestmove") != string::npos) {  // Extracts the best move
            bestMove = bestMove.substr(9, 4); // Extracts move like "b7b6"
        }
        
        int fromCol = bestMove[0] - 'a';  // Converts the best move b7b6 something like this into [1,1] readable integers
        int fromRow = 8 - (bestMove[1] - '0');
        int toCol = bestMove[2] - 'a';
        int toRow = 8 - (bestMove[3] - '0');
        
        movePiece(fromRow, fromCol, toRow, toCol); // Calls the movePiece function to move the best move
    }
    
    // Function to print the board 
    void printBoard() const { 
        for (const auto& row : board) { // For loop iterates through each element of the board vector and prints to the console
            for (char cell : row) {
                cout << cell << " ";
            }
            cout << endl;
        }
    }
};

int main() {
    ChessBoard chess;
    chess.printBoard();

    string fen = chess.getFEN(); // Get current FEN
    int depth = 15; // Example depth
    StockfishAPI::analyzePosition(fen, depth); // Get best move from Stockfish
    
    cout << "\nFEN: " << chess.getFEN() << endl; // Display FEN notation
    
    cout << "\nApplying best move from JSON...\n";
    chess.applyBestMoveFromJson("stockfish_analysis.json"); // Apply best move
    chess.printBoard(); // Prints the new board
    cout << "\nUpdated FEN: " << chess.getFEN() << endl;
    
    return 0;
}