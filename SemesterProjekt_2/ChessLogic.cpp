// Include classes
#include <iostream>
#include "ChessBoard.h"
#include "StockfishUCI.h"

using namespace std;

// Main function
int main() {
    try {
        // Initiates objects of both classes (Which causes the constructer for both to run)
        ChessBoard chess;   
        StockfishUCI engine;

        // Prints the board and FEN to the console
        chess.printBoard();
        cout << "FEN is: " << chess.getFEN() << endl;

        // Initiates a while true loop for switching between user input and stockfish input
        while (true) {
            // If statement that waits until player has moved to finish
            if (!chess.playerMove()) {  
                cout << "Game ended by user." << endl;
                break;
            }

            // Prints board and FEN again
            chess.printBoard();
            cout << "FEN after move: " << chess.getFEN() << endl;

            // Uses applyBestMoveFromEngine function to apply best move based on move history
            chess.applyBestMoveFromEngine(engine);

            // Prints board and FEN last time
            chess.printBoard();
            cout << "FEN after engine: " << chess.getFEN() << endl;
        }
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}
