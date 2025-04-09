#include <string>
#include <vector>
#include "ChessBoard.h"
#include "MoveValidator.h"
#include "Utill.h"
using namespace std;

MoveValidator::MoveValidator() {}

MoveValidator::~MoveValidator() {}

bool MoveValidator::isValidMove(int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board, const string &activeColor)
{
    std::cout << "MoveValidator::isValidMove functions start" << std::endl;
    char piece = board[fromRow][fromCol];
    std::cout << "Activecolor is:" << activeColor << std::endl;
    std::cout << "piece is: " << piece << std::endl; 
    std::cout << "got the piece" << std::endl;

    if ((activeColor == "w") && isupper(piece))
    {
        cout << "cant move white piece" << endl;
        return false;
    }

    if ((activeColor == "b") && islower(piece))
    {
        cout << "cant move black piece" << endl;
        return false;
    }

    if (piece == 'P' || piece == 'p')
    {
        if (!isValidPawn(piece, fromRow, fromCol, toRow, toCol, board, activeColor))
        {
            cout << "invalid pawn movement" << endl;
            return false;
        }
    }

    if (piece == 'R' || piece == 'r')
    {
        if (!isValidRook(piece, fromRow, fromCol, toRow, toCol, board))
        {
            cout << "Rook cooked" << endl;
            return false;
        }
    }

    if (piece == 'N' || piece == 'n')
    {
        if (!isValidKnight(piece, fromRow, fromCol, toRow, toCol, board))
        {
            cout << "Knight cooked" << endl;
            return false;
        }
    }

    if (piece == 'B' || piece == 'b')
    {
        if (!isValidBishop(piece, fromRow, fromCol, toRow, toCol, board))
        {
            cout << "Bishop cooked" << endl;
            return false;
        }
    }

    if (piece == 'Q' || piece == 'q')
    {
        if (!isValidQueen(piece, fromRow, fromCol, toRow, toCol, board))
        {
            cout << "Queen cooked" << endl;
            return false;
        }
    }

    if (piece == 'K' || piece == 'k')
    {
        if (!isValidKing(piece, fromRow, fromCol, toRow, toCol, board, activeColor))
        {
            cout << "King cooked" << endl;
            return false;
        }
    }

    if (piece == '-')
    {
        cout << "no piece at from position" << endl;
        return false;
    }

    cout << "moveValidator being a good boy" << endl;
    return true;
}

bool MoveValidator::isValidCastle(string activeColor, bool castleBools[], string castle, const vector<vector<char>> &board)
{
    if (activeColor == "w")
    {

        bool underAttackBoolwQ = !underAttack(7, 1, activeColor, board) && !underAttack(7, 2, activeColor, board) && !underAttack(7, 3, activeColor, board);
        bool pathClearwQ = board[7][1] == '-' && board[7][2] == '-' && board[7][3] == '-';
        bool castlewQ = castle == "wQ" && castleBools[Utill::WQcastleIndex];

        cout << "castlewQ = " << castlewQ << endl;
        cout << "pathClearwQ = " << pathClearwQ << endl;
        cout << "underAttackBoolwQ = " << underAttackBoolwQ << endl;
        
        if (castlewQ && pathClearwQ && underAttackBoolwQ)
        {

            return true;
        }


        bool underAttackBoolwK = !underAttack(7, 5, activeColor, board) && !underAttack(7, 6, activeColor, board);
        bool pathClearwK = board[7][5] == '-' && board[7][6] == '-';
        bool castlewK = castle == "wK" && castleBools[Utill::WKcastleIndex];
        
        cout << "castlewK = " << castlewK << endl;
        cout << "pathClearwK = " << pathClearwK << endl;
        cout << "underAttackBoolwK = " << underAttackBoolwK << endl;

        if (castlewK && pathClearwK && underAttackBoolwK)
        {
            cout << "Castle is valid" << endl;
            cout << "castleBools[Utill::WQcastleIndex] = " << castleBools[Utill::WQcastleIndex] << endl;
            cout << "castleBools[Utill::WKcastleIndex] = " << castleBools[Utill::WKcastleIndex] << endl;
            cout << "board[7][1] = " << board[7][1] << endl;
            cout << "board[7][2] = " << board[7][2] << endl;
            cout << "board[7][3] = " << board[7][3] << endl;
            return true;
        }  
    }

    else
    {
        bool underAttackBoolbQ = !underAttack(0, 1, activeColor, board) && !underAttack(0, 2, activeColor, board) && !underAttack(0, 3, activeColor, board);
        bool pathClearbQ = board[0][1] == '-' && board[0][2] == '-' && board[0][3] == '-';
        bool castlebQ = castle == "bQ" && castleBools[Utill::BQcastleIndex];
        
        cout << "castlebQ = " << castlebQ << endl;
        cout << "pathClearbQ = " << pathClearbQ << endl;
        cout << "underAttackBoolbQ = " << underAttackBoolbQ << endl;
        if (castlebQ && pathClearbQ && underAttackBoolbQ)
        {
            return true;
        }

        bool underAttackBoolbK = !underAttack(0, 5, activeColor, board) && !underAttack(0, 6, activeColor, board);
        bool pathClearbK = board[0][5] == '-' && board[0][6] == '-';
        bool castlebK = castle == "bK" && castleBools[Utill::BKcastleIndex];

        cout << "castlebK = " << castlebK << endl;
        cout << "pathClearbK = " << pathClearbK << endl;
        cout << "underAttackBoolbK = " << underAttackBoolbK << endl;

        if (castlebK &&
            pathClearbK &&
            underAttackBoolbK)
        {
            return true;
        }
    }
    return false;
}

bool MoveValidator::isValidPromotion(int fromRow, int fromCol, int toCol, const vector<vector<char>> &board)
{
    char piece = board[fromRow][fromCol];

    if ((piece == 'P') && (toCol == 7))
    {
        return true;
    }

    if ((piece == 'p') && (toCol == 0))
    {
        return true;
    }

    return false;
}

/*
bool MoveValidator::isValidEnPassant(string lastMove, int fromRow, int fromCol, string activeColor, int toRow, int toCol, const vector<vector<char>> &board)
{
    char piece = board[fromRow][fromCol];

    if (piece == 'P') {
        if (lastMove = )
        {
        return true;
        }
    }

    if (piece == 'p') {
        if (lastMove = )
        {
        return true;
        }

    }

    return false;
}
*/

// Function for checking if pawn move is valid (Need to implement isPathClear function)
bool MoveValidator::isValidPawn(char piece, int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board, const string &activeColor)
{

    int rowDiff = toRow - fromRow;
    int colDiff = toCol - fromCol;
    char dest = board[toRow][toCol];

    // Diagonal capture
    if (abs(colDiff) == 1)
    {
        if (activeColor == "w" && rowDiff == -1)
            return isupper(dest);
        if (activeColor == "b" && rowDiff == 1)
            return islower(dest);
        return false;
    }

    // Forward move (must be in same column)
    if (colDiff != 0)
        return false;

    // White forward
    if (activeColor == "w")
    {
        if (rowDiff == -1 && dest == '-')
            return true;
        if (rowDiff == -2 && fromRow == 6 && board[fromRow - 1][fromCol] == '-' && dest == '-')
            return true;
    }

    // Black forward
    if (activeColor == "b")
    {
        if (rowDiff == 1 && dest == '-')
            return true;
        if (rowDiff == 2 && fromRow == 1 && board[fromRow + 1][fromCol] == '-' && dest == '-')
            return true;
    }

    return false;
}

bool MoveValidator::isValidRook(char piece, int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board)
{
    std::cout << "MoveValidator::isValidRook functions start" << std::endl;
    int rowDiff = abs(toRow - fromRow);
    int colDiff = abs(toCol - fromCol);

    if ((colDiff != 0) && (rowDiff != 0))
    {
        cout << "Rook cant move in row direction and col direction" << endl;
        return false; // Rook cant move in row direction and col direction
    }

    if (isupper(board[toRow][toCol]) == isupper(piece) && board[toRow][toCol] != '-')
    {
        cout << "Rook cant move to own piece" << endl;
        cout << "Piece detected is" << board[toRow][toCol] << endl;
        cout << "Piece is at " << toRow << " " << toCol << endl;
        return false;
    }

    // move in row direction
    int startR = min(fromRow, toRow);
    int endR = max(fromRow, toRow);
    for (startR + 1; startR < endR; startR++)
    {
        if (board[startR][fromCol] != '-')
        {

            cout << "Rook path is blocked, along row" << endl;
            cout << "Piece detected is" << board[startR][fromCol] << endl;
            cout << "Piece is at " << startR << " " << fromCol << endl;
            return false;
        }
    }

    // move in col direction
    int startC = min(fromCol, toCol);
    int endC = max(fromCol, toCol);
    for (startC + 1; startC < endC; startC++)
    {
        if (board[startC][fromRow] != '-')
        {
            cout << "Rook path is blocked, along col" << endl;
            return false;
        }
    }
    return true;
}

bool MoveValidator::isValidKnight(char piece, int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board)
{
    int rowDiff = abs(toRow - fromRow);
    int colDiff = abs(toCol - fromCol);

    // Knight moves in L shape
    char dest = board[toRow][toCol];
    if ((isupper(piece) != isupper(dest)) && (rowDiff == 2 && colDiff == 1) || (rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2) || (rowDiff == 1 && colDiff == 2))
        return true; // It's only a valid move if the horse moves in an L shape and doesn't land on a piece of it's own color
    return false;
}

bool MoveValidator::isValidBishop(char piece, int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board)
{
    int rowDiff = abs(toRow - fromRow);
    int colDiff = abs(toCol - fromCol);

    char dest = board[toRow][toCol];

    if (rowDiff != colDiff)
    {
        return false; // Bishop must move diagonally
    }

    if (isupper(piece) != islower(dest))
    {
        return false; // Cannot capture own piece
    }

    // Check if path is clear
    int rowStep = (toRow > fromRow) ? 1 : -1;
    int colStep = (toCol > fromCol) ? 1 : -1;
    for (int i = 1; i < rowDiff; ++i)
    {
        if (board[fromRow + i * rowStep][fromCol + i * colStep] != '-')
        {
            return false; // Path is blocked
        }
    }
    return true;
}

bool MoveValidator::isValidQueen(char piece, int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board)
{
    if (isValidBishop(piece, fromRow, fromCol, toRow, toCol, board) ||
        isValidRook(piece, fromRow, fromCol, toRow, toCol, board))
    {
        return true;
    }
    return false;
}

bool MoveValidator::isValidKing(char piece, int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board, const string &activeColor)
{
    int rowDiff = abs(toRow - fromRow);
    int colDiff = abs(toCol - fromCol);
    char dest = board[toRow][toCol];

    if (rowDiff > 1 || colDiff > 1)
        cout << "King cannot move more than 1 square at once" << endl;
        return false;
    if (dest != '-' && ((activeColor == "w" && isupper(dest)) || (activeColor == "b" && islower(dest))))
        cout << "King cannot into piece of own color" << endl;
        return false;

    // Check king cannot move into chess
    if (underAttack)
    {
        cout << "King cannot move into check" << endl;
        return false;
    }

    return true;
}

bool MoveValidator::underAttack(int row, int col, const string activeColor, const vector<vector<char>> &board)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (board[i][j] != '-')
            {
                char piece = board[i][j];
                string pieceColor = islower(piece) ? "w" : "b";

                if (pieceColor != activeColor)
                {
                    // Check if the piece can attack the target square
                    if (isValidMove(i, j, row, col, board, pieceColor))
                    {
                        return true; // The square is under attack
                    }
                }
            }
        }
    }
    return false;
}