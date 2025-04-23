#include <string>
#include <vector>
#include "ChessBoard.h"
#include "MoveValidator.h"
#include "Utill.h"
#include <rclcpp/rclcpp.hpp>
using namespace std;

MoveValidator::MoveValidator() {}

MoveValidator::~MoveValidator() {}

bool MoveValidator::isValidMove(int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board, const string &activeColor, vector<string> moveHistory)
{
    rclcpp::Logger const logger = rclcpp::get_logger("Move_Validator");
    RCLCPP_DEBUG(logger, "MoveValidator::isValidMove function start");
    char piece = board[fromRow][fromCol];
    RCLCPP_DEBUG(logger, std::string("Activecoler is: ").append(activeColor).c_str());
    RCLCPP_DEBUG(logger, "piece is: " + piece);
    RCLCPP_DEBUG(logger, "got the piece");

    if ((activeColor == "w") && isupper(piece))
    {
        RCLCPP_DEBUG(logger, "cant move white piece");
        return false;
    }

    if ((activeColor == "b") && islower(piece))
    {
        RCLCPP_DEBUG(logger, "cant move black piece");
        return false;
    }

    if (piece == 'P' || piece == 'p')
    {
        if (!isValidPawn(piece, fromRow, fromCol, toRow, toCol, board, activeColor, moveHistory))
        {
            RCLCPP_DEBUG(logger, "invalid pawn movement");
            return false;
        }
        {
            RCLCPP_DEBUG(logger, "invalid pawn movement");
            return false;
        }
    }

    if (piece == 'R' || piece == 'r')
    {
        if (!isValidRook(piece, fromRow, fromCol, toRow, toCol, board))
        {
            RCLCPP_DEBUG(logger, "Rook cooked");
            return false;
        }
    }

    if (piece == 'N' || piece == 'n')
    {
        if (!isValidKnight(piece, fromRow, fromCol, toRow, toCol, board))
        {
            RCLCPP_DEBUG(logger, "Knight cooked");
            return false;
        }
    }

    if (piece == 'B' || piece == 'b')
    {
        if (!isValidBishop(piece, fromRow, fromCol, toRow, toCol, board))
        {
            RCLCPP_DEBUG(logger, "Bishop cooked");
            return false;
        }
    }

    if (piece == 'Q' || piece == 'q')
    {
        if (!isValidQueen(piece, fromRow, fromCol, toRow, toCol, board))
        {
            RCLCPP_DEBUG(logger, "Queen cooked");
            return false;
        }
    }

    if (piece == 'K' || piece == 'k')
    {
        if (!isValidKing(piece, fromRow, fromCol, toRow, toCol, board, activeColor))
        {
            RCLCPP_DEBUG(logger, "King cooked");
            return false;
        }
    }

    if (piece == '-')
    {
        RCLCPP_DEBUG(logger, "no piece at from position");
        return false;
    }

    RCLCPP_DEBUG(logger, "moveValidator being a good boy");
    return true;
}

bool MoveValidator::isValidCastle(string activeColor, bool castleBools[], string castle, const vector<vector<char>> &board)
{
    rclcpp::Logger const logger = rclcpp::get_logger("Move_Validator");
    vector<string> nullHistory;
    if (activeColor == "w")
    {

        bool underAttackBoolwQ = !underAttack(7, 1, activeColor, board, nullHistory) && !underAttack(7, 2, activeColor, board, nullHistory) && !underAttack(7, 3, activeColor, board, nullHistory);
        bool pathClearwQ = board[7][1] == '-' && board[7][2] == '-' && board[7][3] == '-';
        bool castlewQ = castle == "wQ" && castleBools[Utill::WQcastleIndex];

        RCLCPP_DEBUG(logger, "castlewQ = " + castlewQ);
        RCLCPP_DEBUG(logger, "pathClearwQ = " + pathClearwQ);
        RCLCPP_DEBUG(logger, "underAttackBoolwQ = " + underAttackBoolwQ);

        if (castlewQ && pathClearwQ && underAttackBoolwQ)
        {

            return true;
        }


        bool underAttackBoolwK = !underAttack(7, 5, activeColor, board, nullHistory) && !underAttack(7, 6, activeColor, board, nullHistory);
        bool pathClearwK = board[7][5] == '-' && board[7][6] == '-';
        bool castlewK = castle == "wK" && castleBools[Utill::WKcastleIndex];
        
        RCLCPP_DEBUG(logger, "castlewK = " + castlewK);
        RCLCPP_DEBUG(logger, "pathClearwK = " + pathClearwK);
        RCLCPP_DEBUG(logger, "underAttackBoolwK = " + underAttackBoolwK);

        if (castlewK && pathClearwK && underAttackBoolwK)
        {
            RCLCPP_DEBUG(logger, "Castle is valid");
            RCLCPP_DEBUG(logger, "castleBools[Utill::WQcastleIndex] = " + castleBools[Utill::WQcastleIndex]);
            RCLCPP_DEBUG(logger, "castleBools[Utill::WKcastleIndex] = " + castleBools[Utill::WKcastleIndex]);
            RCLCPP_DEBUG(logger, "board[7][1] = " + board[7][1]);
            RCLCPP_DEBUG(logger, "board[7][2] = " + board[7][2]);
            RCLCPP_DEBUG(logger, "board[7][3] = " + board[7][3]); 
            return true;
        }  
    }

    else
    {
        bool underAttackBoolbQ = !underAttack(0, 1, activeColor, board, nullHistory) && !underAttack(0, 2, activeColor, board, nullHistory) && !underAttack(0, 3, activeColor, board, nullHistory);
        bool pathClearbQ = board[0][1] == '-' && board[0][2] == '-' && board[0][3] == '-';
        bool castlebQ = castle == "bQ" && castleBools[Utill::BQcastleIndex];
        
        RCLCPP_DEBUG(logger, "castlebQ = " + castlebQ);
        RCLCPP_DEBUG(logger, "pathClearbQ = " + pathClearbQ);
        RCLCPP_DEBUG(logger, "underAttackBoolbQ = " + underAttackBoolbQ);
        if (castlebQ && pathClearbQ && underAttackBoolbQ)
        {
            return true;
        }

        bool underAttackBoolbK = !underAttack(0, 5, activeColor, board, nullHistory) && !underAttack(0, 6, activeColor, board, nullHistory);
        bool pathClearbK = board[0][5] == '-' && board[0][6] == '-';
        bool castlebK = castle == "bK" && castleBools[Utill::BKcastleIndex];

        RCLCPP_DEBUG(logger, "castlebK = " + castlebK);
        RCLCPP_DEBUG(logger, "pathClearbK = " + pathClearbK);
        RCLCPP_DEBUG(logger, "underAttackBoolbK = " + underAttackBoolbK);

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

bool MoveValidator::isValidEnPassant(const string &activeColor, const vector<vector<char>> &board, string currentMove, vector<string> moveHistory)
{
    string lastMove = moveHistory.back();
    
    if (activeColor == "w" && 
       ((lastMove == "b7b5" && currentMove == "a5b6") || 
        (lastMove == "a7a5" && currentMove == "b5a6") || (lastMove == "c7c5" && currentMove == "b5c6") || 
        (lastMove == "b7b5" && currentMove == "c5d6") || (lastMove == "d7d5" && currentMove == "c5d6") || 
        (lastMove == "c7c5" && currentMove == "d5c6") || (lastMove == "e7e5" && currentMove == "d5e6") ||
        (lastMove == "d7d5" && currentMove == "e5d6") || (lastMove == "f7f5" && currentMove == "e5f6") ||
        (lastMove == "e7e5" && currentMove == "f5e6") || (lastMove == "g7g5" && currentMove == "f5g6") ||
        (lastMove == "f7f5" && currentMove == "g5f6") || (lastMove == "h7h5" && currentMove == "g5h6") ||
        (lastMove == "g7g5" && currentMove == "h5g6")))
    {
        return true;
    }

    if (activeColor == "b" && 
       ((lastMove == "b2b4" && currentMove == "a4b3") || 
        (lastMove == "a2a4" && currentMove == "b4a3") || (lastMove == "c2c4" && currentMove == "b4c3") ||
        (lastMove == "b2b4" && currentMove == "c4d3") || (lastMove == "d2d4" && currentMove == "c4d3") ||
        (lastMove == "c2c4" && currentMove == "d4c3") || (lastMove == "e2e4" && currentMove == "d4e3") ||
        (lastMove == "d2d4" && currentMove == "e4d3") || (lastMove == "f2f4" && currentMove == "e4f3") ||
        (lastMove == "e2e4" && currentMove == "f4e3") || (lastMove == "g2g4" && currentMove == "f4g3") ||
        (lastMove == "f2f4" && currentMove == "g4f3") || (lastMove == "h2h4" && currentMove == "g4h3") ||
        (lastMove == "g2g4" && currentMove == "h4g3")))
    {
        return true;
    }
    
    return false;
}

// Function for checking if pawn move is valid (Need to implement isPathClear function)
bool MoveValidator::isValidPawn(char piece, int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board, const string &activeColor, vector<string> moveHistory)
{

    int rowDiff = toRow - fromRow;
    int colDiff = toCol - fromCol;
    char dest = board[toRow][toCol];


    string stringMove = Utill::translateIntMoveToString(fromRow * 1000 + fromCol * 100 + toRow * 10 + toCol);
    RCLCPP_DEBUG(rclcpp::get_logger("Move_Validator"), "I hope the move is %s", stringMove.c_str());

    if (isValidEnPassant(activeColor, board, stringMove, moveHistory))
    {
        return true;
    }else
    {
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
}

bool MoveValidator::isValidRook(char piece, int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board)
{
    rclcpp::Logger const logger = rclcpp::get_logger("Move_Validator");
    RCLCPP_DEBUG(logger, "MoveValidator::isValidRook function start");
    int rowDiff = abs(toRow - fromRow);
    int colDiff = abs(toCol - fromCol);

    if ((colDiff != 0) && (rowDiff != 0))
    {
        RCLCPP_DEBUG(logger, "Rook cant move in row direction and col direction");
        return false; // Rook cant move in row direction and col direction
    }

    if (isupper(board[toRow][toCol]) == isupper(piece) && board[toRow][toCol] != '-')
    {
        RCLCPP_DEBUG(logger, "Rook cant move to own piece");
        RCLCPP_DEBUG(logger, "Piece detected is" + board[toRow][toCol]);
        RCLCPP_DEBUG(logger, "Piece is at %d %d", toRow, toCol);
        return false;
    }
    
    // move in row direction
    int startR = min(fromRow, toRow);
    int endR = max(fromRow, toRow);
    for (startR + 1; startR < endR; startR++)
    {
        if (board[startR][fromCol] != '-')
        {
            RCLCPP_DEBUG(logger, "Rook path is blocked, along row");
            RCLCPP_DEBUG(logger, "Piece detected is" + board[startR][fromCol]);
            RCLCPP_DEBUG(logger, "Piece is at %d %d", startR, fromCol);

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
            RCLCPP_DEBUG(logger, "Rook path is blocked, along col");
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
    rclcpp::Logger const logger = rclcpp::get_logger("Move_Validator");
    int rowDiff = abs(toRow - fromRow);
    int colDiff = abs(toCol - fromCol);
    char dest = board[toRow][toCol];

    if (rowDiff > 1 || colDiff > 1)
        RCLCPP_DEBUG(logger, "King cannot move more than 1 square at once");
        return false;
    if (dest != '-' && ((activeColor == "w" && isupper(dest)) || (activeColor == "b" && islower(dest))))
        RCLCPP_DEBUG(logger, "King cannot move into piece of own coler");
        return false;

    // Check king cannot move into chess
    if (underAttack)
    {
        RCLCPP_DEBUG(logger, "King cannot move into check");
        return false;
    }

    return true;
}

bool MoveValidator::underAttack(int row, int col, const string &activeColor, const vector<vector<char>> &board, vector<string> moveHistory)
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
                    if (isValidMove(i, j, row, col, board, pieceColor, moveHistory))
                    {
                        RCLCPP_DEBUG(rclcpp::get_logger("Move_Validator"), "Square is under attack by piece at %d %d", i, j);
                    
                        return true; // The square is under attack
                    }
                }
            }
        }
    }
    return false;
}