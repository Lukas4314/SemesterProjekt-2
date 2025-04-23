#include "MoveValidator.h"
#include <iostream>
#include <string>
#include <ChessBoard.h>

using namespace std;

int main() {
    ChessBoard chessBoard;
    chessBoard.setBoard(
        // Example setup: You can modify this to set up the board in a specific state
        vector<vector<char>>{
            {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
            {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
            {'-', '-', '-', '-', '-', '-', '-', '-'},
            {'-', '-', '-', '-', '-', '-', '-', '-'},
            {'-', '-', 'P', 'P', 'P', 'P', 'P', '-'},
            {'-', '-', '-', '-', '-', '-', '-', '-'},
            {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
            {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
        }
    );
    cout << chessBoard.applyMoveStringCamera("e4e5") << endl;
    cout << chessBoard.applyMoveStringCamera("f2f4") << endl;
    cout << chessBoard.applyMoveStringCamera("e5f4") << endl;

    return 0;
}
