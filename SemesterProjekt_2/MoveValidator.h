#ifndef MOVEVALIDATOR_H
#define MOVEVALIDATOR_H

#include <string>
#include <vector>
#include <cctype>

using namespace std;

class MoveValidator {
public:
    // Entry point: validates any move
    static bool isValidMove(char piece, int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>>& board, const string& activeColor) {
                
        if ((activeColor == "w") && islower(piece)) return false;
        if ((activeColor == "b") && isupper(piece)) return false;

        if (!isValidPawn(piece, fromRow, fromCol, toRow, fromRow, board, activeColor)) return false;
        if (!isValidRook(piece, fromRow, fromCol, toRow, fromRow, board)) return false;
        if (!isValidKnight(piece, fromRow, fromCol, toRow, toCol)) return false;
        if (!isValidBishop(piece, fromRow, fromCol, toRow, toCol, board)) return false;
        if (!isValidQueen(piece, fromRow, fromCol, toRow, toCol, board)) return false;
        if (!isValidKing(piece, fromRow, fromCol, toRow, toCol)) return false;

        return true;
 
    }

private:

// Function for checking if pawn move is valid (Need to implement isPathClear function)
    static bool isValidPawn(char piece, int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>>& board, const string& activeColor) {

    int moved = toRow - fromRow;
    int colDiff = abs(toCol - fromCol);
    char dest = board[toRow][toCol];

    // Diagonal capture
    if (activeColor == "w" && moved == -1 && colDiff == 1) {
        return islower(dest); // capture black piece
    }

    if (activeColor == "b" && moved == 1 && colDiff == 1) {
        return isupper(dest); // capture white piece
    }

    // Regular forward move (no column change)
    if (colDiff != 0) return false; // can't move sideways unless capturing

    // Check direction + length
    if (activeColor == "w") {
        if (moved >= 0) return false;
        moved = -moved;
        if (moved == 2 && fromRow != 6) return false;
        if (moved > 2) return false;

        // Blocked path check
        if (moved == 1 && board[toRow][toCol] != '-') return false;
        if (moved == 2 && (board[fromRow - 1][fromCol] != '-' || board[toRow][toCol] != '-')) return false;
    }

    if (activeColor == "b") {
        if (moved <= 0) return false;
        if (moved == 2 && fromRow != 1) return false;
        if (moved > 2) return false;

        // Blocked path check
        if (moved == 1 && board[toRow][toCol] != '-') return false;
        if (moved == 2 && (board[fromRow + 1][fromCol] != '-' || board[toRow][toCol] != '-')) return false;
    }

    return true;
}

    static bool isValidRook(char piece, int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>>& board) {

        int moved = toRow - fromRow;
        int colDiff = abs(toCol - fromCol);
        char dest = board[toRow][toCol];

        if ((colDiff != 0) && (moved != 0)) return false; // Rook cant move in row direction and col direction



    }

    static bool isValidKnight(char piece, int fromRow, int fromCol, int toRow, int toCol);
    static bool isValidBishop(char piece, int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>>& board);
    static bool isValidQueen(char piece, int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>>& board);
    static bool isValidKing(char piece, int fromRow, int fromCol, int toRow, int toCol);
    
    static bool isPathClear(int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>>& board);
};

#endif
