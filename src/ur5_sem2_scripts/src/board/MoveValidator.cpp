#include <string>
#include <vector>
#include "ChessBoard.h"
#include "MoveValidator.h"
#include "Utill.h"
#include <rclcpp/rclcpp.hpp>

using namespace std;

MoveValidator::MoveValidator() {}

MoveValidator::~MoveValidator() {}

bool MoveValidator::correctColor(char piece, string &activeColor)
{
    if ((activeColor == "w") && ChessBoard::isBlack(piece))
    {
        return false;
    }

    if ((activeColor == "b") && ChessBoard::isWhite(piece))
    {
        return false;
    }
    return true;
}

bool MoveValidator::isValidMove(int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board, string &activeColor, bool calledByUnderAttack, bool calledByKing)
{
    rclcpp::Logger const logger = rclcpp::get_logger("Move_Validator");

    char piece = board[fromRow][fromCol];

    if (!correctColor(piece, activeColor))
    {
        return false;
    }

    if (fromRow == toRow && fromCol == toCol)
    {
        return false; // No movement
    }

    if (piece == 'P' || piece == 'p')
    {
        if (!isValidPawn(fromRow, fromCol, toRow, toCol, board, activeColor, calledByUnderAttack))
        {
            return false;
        }
    }

    if (piece == 'R' || piece == 'r')
    {
        if (!isValidRook(piece, fromRow, fromCol, toRow, toCol, board))
        {
            return false;
        }
    }

    if (piece == 'N' || piece == 'n')
    {
        if (!isValidKnight(piece, fromRow, fromCol, toRow, toCol, board))
        {
            return false;
        }
    }

    if (piece == 'B' || piece == 'b')
    {
        if (!isValidBishop(piece, fromRow, fromCol, toRow, toCol, board))
        {
            return false;
        }
    }

    if (piece == 'Q' || piece == 'q')
    {
        if (!isValidQueen(piece, fromRow, fromCol, toRow, toCol, board))
        {
            return false;
        }
    }

    if (piece == 'K' || piece == 'k')
    {
        if (!isValidKing(piece, fromRow, fromCol, toRow, toCol, board, activeColor, calledByKing))
        {
            return false;
        }
    }

    if (piece == '-')
    {
        return false;
    }

    // Returns if called by under attack since it otherwise would leave a recursion loop
    if (calledByUnderAttack)
    {
        return true;
    }

    // Checks that moving a piece does not leave the king in check
    vector<vector<char>> boardCopy = board;

    boardCopy[toRow][toCol] = boardCopy[fromRow][fromCol];
    boardCopy[fromRow][fromCol] = '-';

    vector<int> kingPos = findKing(activeColor, boardCopy);

    if (underAttack(kingPos[0], kingPos[1], activeColor, boardCopy))
    {
        RCLCPP_DEBUG(logger, "King is left in check");
        return false;
    }
    RCLCPP_DEBUG(logger, "MoveValidator says move is valid");
    return true;
}

vector<int> MoveValidator::findKing(string color, const vector<vector<char>> &board)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (board[i][j] == 'K' && color == "b")
            {
                return {i, j};
            }
            else if (board[i][j] == 'k' && color == "w")
            {
                return {i, j};
            }
        }
    }
    return {-1, -1}; // King not found
}

bool MoveValidator::checkHistoryForKingOrRookMovement(const string &move, const vector<string> &moveHistory)
{
    for (int i = 0; i < (int)moveHistory.size(); i++)
    {
        bool whiteKingHasMoved = (moveHistory[i].substr(0, 2) == "e1");
        bool blackKingHasMoved = (moveHistory[i].substr(0, 2) == "e8");

        bool whiteLeftRookHadMoved = (moveHistory[i].substr(0, 2) == "a1");
        bool whiteRightRookHadMoved = (moveHistory[i].substr(0, 2) == "h1");
        bool blackLeftRookHadMoved = (moveHistory[i].substr(0, 2) == "a8");
        bool blackRightRookHadMoved = (moveHistory[i].substr(0, 2) == "h8");

        if (move == "wK" && (whiteKingHasMoved || whiteRightRookHadMoved))
        {
            return true;
        }

        if (move == "wQ" && (whiteKingHasMoved || whiteLeftRookHadMoved))
        {
            return true;
        }
        if (move == "bK" && (blackKingHasMoved || blackRightRookHadMoved))
        {
            return true;
        }
        if (move == "bQ" && (blackKingHasMoved || blackLeftRookHadMoved))
        {
            return true;
        }
    }
    return false; // No king or rook movement found in history
}

bool MoveValidator::castlePathUnderAttack(const string &move, const vector<vector<char>> &board)
{
    if (move == "wK")
    {
        return (underAttack(7, 5, "w", board) || underAttack(7, 6, "w", board) || underAttack(7, 4, "w", board));
    }
    if (move == "wQ")
    {
        return (underAttack(7, 3, "w", board) || underAttack(7, 2, "w", board) || underAttack(7, 4, "w", board));
    }
    if (move == "bK")
    {
        return (underAttack(0, 5, "b", board) || underAttack(0, 6, "b", board) || underAttack(0, 4, "b", board));
    }
    if (move == "bQ")
    {
        return (underAttack(0, 3, "b", board) || underAttack(0, 2, "b", board) || underAttack(0, 4, "b", board));
    }

    return false;
}

bool MoveValidator::emptySquaresForCastle(const string &move, const vector<vector<char>> &board)
{
    if (move == "wK")
    {
        return (board[7][5] == '-' && board[7][6] == '-');
    }

    if (move == "wQ")
    {
        return (board[7][3] == '-' && board[7][2] == '-' && board[7][1] == '-');
    }
    if (move == "bK")
    {
        return (board[0][5] == '-' && board[0][6] == '-');
    }
    if (move == "bQ")
    {
        return (board[0][3] == '-' && board[0][2] == '-' && board[0][1] == '-');
    }

    return false;
}

bool MoveValidator::isValidCastle(string activeColor, string move, const vector<vector<char>> &board, const vector<string> &moveHistory)
{

    // Check if the move is for the correct color
    if (move[0] != activeColor[0])
    {
        return false;
    }

    if (checkHistoryForKingOrRookMovement(move, moveHistory))
    {
        return false;
    }

    if (castlePathUnderAttack(move, board))
    {
        return false;
    }

    if (!emptySquaresForCastle(move, board))
    {
        return false;
    }

    return true;
}

bool MoveValidator::isValidEnPassant(string &activeColor, const string &currentMove, const vector<string> &moveHistory)
{
    if (moveHistory.empty())
    {
        return false; // No previous moves to check
    }

    string lastMove = moveHistory.back();

    if (activeColor == "w")
    {
        if (lastMove == "b7b5" && currentMove == "a5b6")
        {
            return true;
        }
        if (lastMove == "a7a5" && currentMove == "b5a6")
        {
            return true;
        }
        if (lastMove == "c7c5" && currentMove == "b5c6")
        {
            return true;
        }
        if (lastMove == "b7b5" && currentMove == "c5d6")
        {
            return true;
        }
        if (lastMove == "d7d5" && currentMove == "c5d6")
        {
            return true;
        }
        if (lastMove == "c7c5" && currentMove == "d5c6")
        {
            return true;
        }
        if (lastMove == "e7e5" && currentMove == "d5e6")
        {
            return true;
        }
        if (lastMove == "d7d5" && currentMove == "e5d6")
        {
            return true;
        }
        if (lastMove == "f7f5" && currentMove == "e5f6")
        {
            return true;
        }
        if (lastMove == "e7e5" && currentMove == "f5e6")
        {
            return true;
        }
        if (lastMove == "g7g5" && currentMove == "f5g6")
        {
            return true;
        }
        if (lastMove == "f7f5" && currentMove == "g5f6")
        {
            return true;
        }
        if (lastMove == "h7h5" && currentMove == "g5h6")
        {
            return true;
        }
        if (lastMove == "g7g5" && currentMove == "h5g6")
        {
            return true;
        }
    }
    else if (activeColor == "b")
    {
        if (lastMove == "b2b4" && currentMove == "a4b3")
        {
            return true;
        }
        if (lastMove == "a2a4" && currentMove == "b4a3")
        {
            return true;
        }
        if (lastMove == "c2c4" && currentMove == "b4c3")
        {
            return true;
        }
        if (lastMove == "b2b4" && currentMove == "c4d3")
        {
            return true;
        }
        if (lastMove == "d2d4" && currentMove == "c4d3")
        {
            return true;
        }
        if (lastMove == "c2c4" && currentMove == "d4c3")
        {
            return true;
        }
        if (lastMove == "e2e4" && currentMove == "d4e3")
        {
            return true;
        }
        if (lastMove == "d2d4" && currentMove == "e4d3")
        {
            return true;
        }
        if (lastMove == "f2f4" && currentMove == "e4f3")
        {
            return true;
        }
        if (lastMove == "e2e4" && currentMove == "f4e3")
        {
            return true;
        }
        if (lastMove == "g2g4" && currentMove == "f4g3")
        {
            return true;
        }
        if (lastMove == "f2f4" && currentMove == "g4f3")
        {
            return true;
        }
        if (lastMove == "h2h4" && currentMove == "g4h3")
        {
            return true;
        }
        if (lastMove == "g2g4" && currentMove == "h4g3")
        {

            return true;
        }
    }
    return false;
}

// Function for checking if pawn move is valid (Need to implement isPathClear function)
bool MoveValidator::isValidPawn(int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board, string &activeColor, bool calledByUnderAttack)
{
    int rowDiff = toRow - fromRow;
    int colDiff = toCol - fromCol;
    char dest = board[toRow][toCol];

    // Diagonal capture
    if (abs(colDiff) == 1)
    {
        if (activeColor == "w" && rowDiff == -1)
        {
            return ChessBoard::isBlack(dest);
        }
        if (activeColor == "b" && rowDiff == 1)
        {
            return ChessBoard::isWhite(dest);
        }
        return false;
    }

    // Forward move (must be in same column)
    if (colDiff != 0)
    {
        return false;
    }

    // Ensures the pawn doesnt count as attacking a square that is infront of it
    if (calledByUnderAttack)
    {
        return false;
    }

    
    // White forward
    if (activeColor == "w")
    {
        if (rowDiff == -1 && dest == '-')
        {
            return true;
        }
        if (rowDiff == -2 && fromRow == 6 && board[fromRow - 1][fromCol] == '-' && dest == '-')
        {
            return true;
        }
    }

    // Black forward
    if (activeColor == "b")
    {
        if (rowDiff == 1 && dest == '-')
        {
            return true;
        }
        if (rowDiff == 2 && fromRow == 1 && board[fromRow + 1][fromCol] == '-' && dest == '-')
        {
            return true;
        }
    }
    return false;
}

bool MoveValidator::pathClearBetweenOrthogonal(int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board)
{
    int absRowDiff = abs(toRow - fromRow);
    int absColDiff = abs(toCol - fromCol);

    // Movement along collumn
    if (absRowDiff == 0)
    {
        int startCol = min(fromCol, toCol);
        int endCol = max(fromCol, toCol);
        for (int col = startCol + 1; col < endCol; ++col)
        {
            if (col == endCol)
            {
                break;
            }
            if (board[fromRow][col] != '-')
            {
                return false; // Path is blocked
            }
        }
    }
    // Movement along row
    else if (absColDiff == 0)
    {
        int startRow = min(fromRow, toRow);
        int endRow = max(fromRow, toRow);
        for (int row = startRow + 1; row < endRow; ++row)
        {
            if (row == endRow)
            {
                break;
            }
            if (board[row][fromCol] != '-')
            {
                return false; // Path is blocked
            }
        }
    }
    return true;
}

bool MoveValidator::pathClearBetweenDiagonal(int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board)
{
    int rowDiff = abs(toRow - fromRow);

    // Check if the row and col should increase or decrease
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

bool MoveValidator::isValidRook(char piece, int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board)
{
    int absRowDiff = abs(toRow - fromRow);
    int absColDiff = abs(toCol - fromCol);
    char dest = board[toRow][toCol];

    // Checks for diagonal movement
    if ((absColDiff != 0) && (absRowDiff != 0))
    {
        return false;
    }

    // Checks if we move to the same color, and if the destination is not empty since then we run isWhite() on a empty square
    if (ChessBoard::isWhite(dest) == ChessBoard::isWhite(piece) && dest != '-')
    {
        return false;
    }

    // Checks for clear path
    if (!pathClearBetweenOrthogonal(fromRow, fromCol, toRow, toCol, board))
    {
        return false;
    }

    return true;
}

bool MoveValidator::isValidKnight(char piece, int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board)
{
    int rowDiff = abs(toRow - fromRow);
    int colDiff = abs(toCol - fromCol);
    char dest = board[toRow][toCol];

    // Knight moves in L shape
    if ((ChessBoard::isBlack(piece) != ChessBoard::isBlack(dest)) && (rowDiff == 2 && colDiff == 1) || (rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2) || (rowDiff == 1 && colDiff == 2))
        return true; // It's only a valid move if the horse moves in an L shape and doesn't land on a piece of it's own color
    return false;
}

bool MoveValidator::isValidBishop(char piece, int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board)
{
    int rowDiff = abs(toRow - fromRow);
    int colDiff = abs(toCol - fromCol);
    char dest = board[toRow][toCol];

    // Bishop must move diagonally
    if (rowDiff != colDiff)
    {
        return false;
    }

    // If dest is not empty and both are the same color, error cant capture own piece
    if (dest != '-' && ((ChessBoard::isBlack(piece) == ChessBoard::isBlack(dest))))
    {
        return false;
    }

    // Check if path is clear
    if (!pathClearBetweenDiagonal(fromRow, fromCol, toRow, toCol, board))
    {
        return false;
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

bool MoveValidator::isValidKing(char piece, int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board, string &activeColor, bool calledByKing)
{
    int rowDiff = abs(toRow - fromRow);
    int colDiff = abs(toCol - fromCol);
    char dest = board[toRow][toCol];

    if (rowDiff > 1 || colDiff > 1)
    {
        return false; // King can only move one square in any direction
    }

    if (dest != '-' && ((ChessBoard::isBlack(piece) == ChessBoard::isBlack(dest))))
    {
        return false; // Can't capture own piece
    }


    if (calledByKing)
    {
        return true; // Called by King underattack, so checking for underattack would become a infinite loop
    }

    if (underAttack(toRow, toCol, activeColor, board, true))
    {
        return false; // Can't move into check
    }
    return true;
}

bool MoveValidator::underAttack(int row, int col, const string &activeColor, const vector<vector<char>> &board, bool calledByKing)
{
    rclcpp::Logger const logger = rclcpp::get_logger("Move_Validator");

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {

            if (board[i][j] == '-')
            {
                continue;
            }

            char piece = board[i][j];
            string pieceColor = ChessBoard::isWhite(piece) ? "w" : "b";

            if (pieceColor == activeColor)
            {
                continue;
            }

            // Check if the opposite colorored piece can attack the target square
            if (!isValidMove(i, j, row, col, board, pieceColor, true, calledByKing))
            {
                continue;
            }

            RCLCPP_DEBUG(logger, "Piece %c at %d, %d can attack %d, %d", piece, i, j, row, col);
            return true;
        }
    }
    return false;
}