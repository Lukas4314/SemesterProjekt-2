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
        string pieces = "RNBQKBNR";
        for (int i = 0; i < 8; ++i) {
            board[0][i] = pieces[i];  
            board[1][i] = 'P';        
            board[6][i] = 'p';        
            board[7][i] = tolower(pieces[i]); 
        }
    }
    
    // Function to update FEN notation based on the current board state
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
            if (i > 0) {
                FEN += "/";
            }
        }
        FEN += " " + activeColor + " " + castlingRights + " " + enPassant + " " + to_string(halfmoveClock) + " " + to_string(fullmoveNumber);
    }
    
    // Function to get the FEN notation
    string getFEN() const {
        return FEN;
    }
    
    // Function to move a piece on the board
    void movePiece(int fromRow, int fromCol, int toRow, int toCol) {
        if (fromRow >= 0 && fromRow < 8 && fromCol >= 0 && fromCol < 8 &&
            toRow >= 0 && toRow < 8 && toCol >= 0 && toCol < 8) {
            board[toRow][toCol] = board[fromRow][fromCol];
            board[fromRow][fromCol] = '-';
            activeColor = (activeColor == "w") ? "b" : "w";
            fullmoveNumber += (activeColor == "w") ? 1 : 0;
            updateFEN();
        } else {
            cerr << "Invalid move!" << endl;
        }
    }
    
    // Function to allow user input for move
    bool playerMove() {
        string move;
        cout << "Enter your move (e.g., a2a4) or type 'quit' to exit: ";
        cin >> move;
        
        if (move == "quit") {
            return false;
        }
        
        if (move.length() == 4) {
            int fromCol = move[0] - 'a';
            int fromRow = 8 - (move[1] - '0');
            int toCol = move[2] - 'a';
            int toRow = 8 - (move[3] - '0');
            
            movePiece(fromRow, fromCol, toRow, toCol);
        } else {
            cout << "Invalid move format!" << endl;
        }
        return true;
    }
    
    // Function to read best move from Stockfish JSON file and apply it
    void applyBestMoveFromJson(const string& filePath) {
        ifstream file(filePath);
        if (!file.is_open()) {
            cerr << "Failed to open JSON file!" << endl;
            return;
        }
        
        nlohmann::json jsonData;
        file >> jsonData;
        file.close();
        
        string bestMove = jsonData["bestmove"].get<string>();
        if (bestMove.find("bestmove") != string::npos) {
            bestMove = bestMove.substr(9, 4);
        }
        
        int fromCol = bestMove[0] - 'a';
        int fromRow = 8 - (bestMove[1] - '0');
        int toCol = bestMove[2] - 'a';
        int toRow = 8 - (bestMove[3] - '0');
        
        movePiece(fromRow, fromCol, toRow, toCol);
    }
    
    // Function to print the board 
    void printBoard() const { 
        for (const auto& row : board) {
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

    while (true) {
        if (!chess.playerMove()) {
            cout << "Game ended by user." << endl;
            break;
        }
        chess.printBoard();
        StockfishAPI::analyzePosition(chess.getFEN(), 15);
        chess.applyBestMoveFromJson("stockfish_analysis.json");
        chess.printBoard();
    }
    return 0;
}
