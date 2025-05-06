#include <string>
#include <vector>
#include "ChessBoard.h"
#include "MoveValidator.h"
#include "Utill.h"
#include <rclcpp/rclcpp.hpp>
using namespace std;

MoveValidator::MoveValidator() {}

MoveValidator::~MoveValidator() {}

bool MoveValidator::isValidMove(int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board, string &activeColor, const vector<string> &moveHistory)
{
    rclcpp::Logger const logger = rclcpp::get_logger("Move_Validator");
    RCLCPP_DEBUG(logger, "MoveValidator::isValidMove function start");
    char piece = board[fromRow][fromCol];
    RCLCPP_DEBUG(logger, std::string("Activecoler is: ").append(activeColor).c_str());
    RCLCPP_DEBUG(logger, string("piece is: ").append(string(1, piece)).c_str());
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
        bool kingStatus = isValidKing(piece, fromRow, fromCol, toRow, toCol, board, activeColor);
        RCLCPP_DEBUG(logger, "kingStatus is:");
        RCLCPP_DEBUG(logger, kingStatus ? "true" : "false");

        if (!kingStatus)
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

bool MoveValidator::isValidCastle(string activeColor, bool castleBools[], string castle, const vector<vector<char>> &board, const vector<string> &moveHistory)
{
    rclcpp::Logger const logger = rclcpp::get_logger("Move_Validator");

    if (activeColor == "w")
    {

        bool underAttackBoolwQ = !underAttack(7, 1, activeColor, board, moveHistory) && !underAttack(7, 2, activeColor, board, moveHistory) && !underAttack(7, 3, activeColor, board, moveHistory);
        bool pathClearwQ = board[7][1] == '-' && board[7][2] == '-' && board[7][3] == '-';
        bool castlewQ = castle == "wQ" && castleBools[Utill::WQcastleIndex];

        RCLCPP_DEBUG(logger, "castlewQ = " + castlewQ);
        RCLCPP_DEBUG(logger, "pathClearwQ = " + pathClearwQ);
        RCLCPP_DEBUG(logger, "underAttackBoolwQ = " + underAttackBoolwQ);

        if (castlewQ && pathClearwQ && underAttackBoolwQ)
        {

            return true;
        }

        bool underAttackBoolwK = !underAttack(7, 5, activeColor, board, moveHistory) && !underAttack(7, 6, activeColor, board, moveHistory);
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
        bool underAttackBoolbQ = !underAttack(0, 1, activeColor, board, moveHistory) && !underAttack(0, 2, activeColor, board, moveHistory) && !underAttack(0, 3, activeColor, board, moveHistory);
        bool pathClearbQ = board[0][1] == '-' && board[0][2] == '-' && board[0][3] == '-';
        bool castlebQ = castle == "bQ" && castleBools[Utill::BQcastleIndex];

        RCLCPP_DEBUG(logger, "castlebQ = " + castlebQ);
        RCLCPP_DEBUG(logger, "pathClearbQ = " + pathClearbQ);
        RCLCPP_DEBUG(logger, "underAttackBoolbQ = " + underAttackBoolbQ);
        if (castlebQ && pathClearbQ && underAttackBoolbQ)
        {
            return true;
        }

        bool underAttackBoolbK = !underAttack(0, 5, activeColor, board, moveHistory) && !underAttack(0, 6, activeColor, board, moveHistory);
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

bool MoveValidator::isValidPromotion(int fromRow, int fromCol, int toRow, const vector<vector<char>> &board)
{
    char piece = board[fromRow][fromCol];

    RCLCPP_DEBUG(rclcpp::get_logger("Move_Validator"), string("piece is: ").append(string(1, piece)).c_str());

    RCLCPP_DEBUG(rclcpp::get_logger("Move_Validator"), string("toRow is: ").append(to_string(toRow)).c_str());

    if ((piece == 'p') && (toRow == 0))
    {
        return true;
    }

    if ((piece == 'P') && (toRow == 7))
    {
        return true;
    }

    return false;
}

bool MoveValidator::isValidEnPassant(string &activeColor, const vector<vector<char>> &board, const string &currentMove, const vector<string> &moveHistory)
{
    rclcpp::Logger const logger = rclcpp::get_logger("Move_Validator");
    RCLCPP_DEBUG(logger, "MoveValidator::isValidEnPassant function start");
    if (moveHistory.empty())
    {
        RCLCPP_DEBUG(logger, "MoveHistory EMPTY");
        return false; // No previous moves to check
    }

    string lastMove = moveHistory.back();

    if (activeColor == "w")
    {
        if (lastMove == "b7b5" && currentMove == "a5b6")
        {
            RCLCPP_DEBUG(logger, "Matched w en passant: b7b5 a5b6");
            return true;
        }
        if (lastMove == "a7a5" && currentMove == "b5a6")
        {
            RCLCPP_DEBUG(logger, "Matched w en passant: a7a5 b5a6");
            return true;
        }
        if (lastMove == "c7c5" && currentMove == "b5c6")
        {
            RCLCPP_DEBUG(logger, "Matched w en passant: c7c5 b5c6");
            return true;
        }
        if (lastMove == "b7b5" && currentMove == "c5d6")
        {
            RCLCPP_DEBUG(logger, "Matched w en passant: b7b5 c5d6");
            return true;
        }
        if (lastMove == "d7d5" && currentMove == "c5d6")
        {
            RCLCPP_DEBUG(logger, "Matched w en passant: d7d5 c5d6");
            return true;
        }
        if (lastMove == "c7c5" && currentMove == "d5c6")
        {
            RCLCPP_DEBUG(logger, "Matched w en passant: c7c5 d5c6");
            return true;
        }
        if (lastMove == "e7e5" && currentMove == "d5e6")
        {
            RCLCPP_DEBUG(logger, "Matched w en passant: e7e5 d5e6");
            return true;
        }
        if (lastMove == "d7d5" && currentMove == "e5d6")
        {
            RCLCPP_DEBUG(logger, "Matched w en passant: d7d5 e5d6");
            return true;
        }
        if (lastMove == "f7f5" && currentMove == "e5f6")
        {
            RCLCPP_DEBUG(logger, "Matched w en passant: f7f5 e5f6");
            return true;
        }
        if (lastMove == "e7e5" && currentMove == "f5e6")
        {
            RCLCPP_DEBUG(logger, "Matched w en passant: e7e5 f5e6");
            return true;
        }
        if (lastMove == "g7g5" && currentMove == "f5g6")
        {
            RCLCPP_DEBUG(logger, "Matched w en passant: g7g5 f5g6");
            return true;
        }
        if (lastMove == "f7f5" && currentMove == "g5f6")
        {
            RCLCPP_DEBUG(logger, "Matched w en passant: f7f5 g5f6");
            return true;
        }
        if (lastMove == "h7h5" && currentMove == "g5h6")
        {
            RCLCPP_DEBUG(logger, "Matched w en passant: h7h5 g5h6");
            return true;
        }
        if (lastMove == "g7g5" && currentMove == "h5g6")
        {
            RCLCPP_DEBUG(logger, "Matched w en passant: g7g5 h5g6");
            return true;
        }
    }
    else if (activeColor == "b")
    {
        if (lastMove == "b2b4" && currentMove == "a4b3")
        {
            RCLCPP_DEBUG(logger, "Matched b en passant: b2b4 a4b3");
            return true;
        }
        if (lastMove == "a2a4" && currentMove == "b4a3")
        {
            RCLCPP_DEBUG(logger, "Matched b en passant: a2a4 b4a3");
            return true;
        }
        if (lastMove == "c2c4" && currentMove == "b4c3")
        {
            RCLCPP_DEBUG(logger, "Matched b en passant: c2c4 b4c3");
            return true;
        }
        if (lastMove == "b2b4" && currentMove == "c4d3")
        {
            RCLCPP_DEBUG(logger, "Matched b en passant: b2b4 c4d3");
            return true;
        }
        if (lastMove == "d2d4" && currentMove == "c4d3")
        {
            RCLCPP_DEBUG(logger, "Matched b en passant: d2d4 c4d3");
            return true;
        }
        if (lastMove == "c2c4" && currentMove == "d4c3")
        {
            RCLCPP_DEBUG(logger, "Matched b en passant: c2c4 d4c3");
            return true;
        }
        if (lastMove == "e2e4" && currentMove == "d4e3")
        {
            RCLCPP_DEBUG(logger, "Matched b en passant: e2e4 d4e3");
            return true;
        }
        if (lastMove == "d2d4" && currentMove == "e4d3")
        {
            RCLCPP_DEBUG(logger, "Matched b en passant: d2d4 e4d3");
            return true;
        }
        if (lastMove == "f2f4" && currentMove == "e4f3")
        {
            RCLCPP_DEBUG(logger, "Matched b en passant: f2f4 e4f3");
            return true;
        }
        if (lastMove == "e2e4" && currentMove == "f4e3")
        {
            RCLCPP_DEBUG(logger, "Matched b en passant: e2e4 f4e3");
            return true;
        }
        if (lastMove == "g2g4" && currentMove == "f4g3")
        {
            RCLCPP_DEBUG(logger, "Matched b en passant: g2g4 f4g3");
            return true;
        }
        if (lastMove == "f2f4" && currentMove == "g4f3")
        {
            RCLCPP_DEBUG(logger, "Matched b en passant: f2f4 g4f3");
            return true;
        }
        if (lastMove == "h2h4" && currentMove == "g4h3")
        {
            RCLCPP_DEBUG(logger, "Matched b en passant: h2h4 g4h3");
            return true;
        }
        if (lastMove == "g2g4" && currentMove == "h4g3")
        {
            RCLCPP_DEBUG(logger, "Matched b en passant: g2g4 h4g3");
            return true;
        }
    }

    RCLCPP_DEBUG(logger, "None of the Enpassant moves match");
    return false;
}

// Function for checking if pawn move is valid (Need to implement isPathClear function)
bool MoveValidator::isValidPawn(char piece, int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board, string &activeColor, const vector<string> &moveHistory)
{

    int rowDiff = toRow - fromRow;
    int colDiff = toCol - fromCol;
    char dest = board[toRow][toCol];

    string stringMove = Utill::translateIntMoveToString(fromRow * 1000 + fromCol * 100 + toRow * 10 + toCol);
    RCLCPP_DEBUG(rclcpp::get_logger("Move_Validator"), "I hope the move is %s", stringMove.c_str());

    if (isValidEnPassant(activeColor, board, stringMove, moveHistory))
    {
        return true;
    }

    else
    {
        // Diagonal capture
        if (abs(colDiff) == 1)
        {
            if (activeColor == "w" && rowDiff == -1)
            {
                return isupper(dest);
            }
            if (activeColor == "b" && rowDiff == 1)
            {
                return islower(dest);
            }
            RCLCPP_DEBUG(rclcpp::get_logger("Move_Validator"), "Pawn cant move in diagonal direction");
            return false;
        }

        // Forward move (must be in same column)
        if (colDiff != 0)
        {
            RCLCPP_DEBUG(rclcpp::get_logger("Move_Validator"), "Pawn cant move in col direction");
            return false;
        }

        // White forward
        if (activeColor == "w")
        {
            if (rowDiff == -1 && dest == '-')
            {
                RCLCPP_DEBUG(rclcpp::get_logger("Move_Validator"), "Pawn can move one step forward");
                return true;
            }
            if (rowDiff == -2 && fromRow == 6 && board[fromRow - 1][fromCol] == '-' && dest == '-')
            {
                RCLCPP_DEBUG(rclcpp::get_logger("Move_Validator"), "Pawn can move two steps forward");
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
        RCLCPP_DEBUG(rclcpp::get_logger("Move_Validator"), "Pawn cant move in row direction");
        return false;
    }
}

bool MoveValidator::isValidRook(char piece, int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board)
{
    rclcpp::Logger const logger = rclcpp::get_logger("Move_Validator");
    RCLCPP_DEBUG(logger, "MoveValidator::isValidRook function start");
    int rowDiff = abs(toRow - fromRow);
    int colDiff = abs(toCol - fromCol);
    RCLCPP_DEBUG(logger, "rowDiff is: %d", rowDiff);
    RCLCPP_DEBUG(logger, "colDiff is: %d", colDiff);

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

    if (rowDiff == 0) {
        cout << "Move in col diff if statement entered" << endl;
        int startR = min(fromRow, toRow);
        int endR = max(fromRow, toRow);
        cout << "StartR is: " << startR << endl;
        cout << "endR is: " << endR << endl;
        cout << "fromCol: " << fromCol << endl;
        cout << "toCol: " << toCol << endl;

        for (fromCol; fromCol > toCol; fromCol++)
        {
            cout << "In for loop" << endl;
            cout << "endCol is: " << toCol << endl;
            cout << fromCol << endl;
            if (board[startR][fromCol] != '-')
            {
                RCLCPP_DEBUG(logger, "Rook path is blocked, along row");
                return false;
            }
        }
    }   

        // move in col direction

    if (colDiff == 0) {
        cout << "Move in col diff if statement entered" << endl;
        int startC = min(fromCol, toCol);
        int endC = max(fromCol, toCol);
        cout << "StartC is: " << startC << endl;
        cout << "endC is: " << endC << endl;
        cout << "fromRow: " << fromRow << endl;
        cout << "toRow: " << toRow << endl;

        for (fromRow; fromRow > toRow; fromRow++)
        {
            cout << "In for loop" << endl;
            cout << "endRow is: " << toRow << endl;
            cout << fromRow << endl;
            if (board[startC][fromRow] != '-')
            {
                RCLCPP_DEBUG(logger, "Rook path is blocked, along col");
                return false;
            }
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
    RCLCPP_DEBUG(rclcpp::get_logger("the dest is "), string(1, dest).c_str());

    if (rowDiff != colDiff)
    {
        RCLCPP_DEBUG(rclcpp::get_logger("Move_Validator"), "Bishop can only move diagonally");
        return false; // Bishop must move diagonally
    }

    // If dest is not empty and both are the same color (based on your convention)
    if (dest != '-' && ((isupper(piece) && isupper(dest)) || (islower(piece) && islower(dest))))
    {
        RCLCPP_DEBUG(rclcpp::get_logger("Move_Validator"), "Bishop cannot capture own piece");
        return false;
    }

    // Check if path is clear
    int rowStep = (toRow > fromRow) ? 1 : -1;
    int colStep = (toCol > fromCol) ? 1 : -1;
    for (int i = 1; i < rowDiff; ++i)
    {
        if (board[fromRow + i * rowStep][fromCol + i * colStep] != '-')
        {
            RCLCPP_DEBUG(rclcpp::get_logger("Move_Validator"), "Bishop path is blocked");
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

bool MoveValidator::isValidKing(char piece, int fromRow, int fromCol, int toRow, int toCol, const vector<vector<char>> &board, string &activeColor)
{
    rclcpp::Logger const logger = rclcpp::get_logger("Move_Validator");
    RCLCPP_DEBUG(logger, "MoveValidator::isValidKing function start");
    int rowDiff = abs(toRow - fromRow);
    int colDiff = abs(toCol - fromCol);
    std::cout << fromRow << fromCol << toRow << toCol << std::endl;
    std::cout << "rowDiff = " << rowDiff << std::endl;
    std::cout << "colDiff = " << colDiff << std::endl;

    char dest = board[toRow][toCol];

    if (rowDiff > 1 || colDiff > 1)
    {
        RCLCPP_DEBUG(logger, "King cannot move more than 1 square at once");
        std::cout << "King cannot move more than 1 square at once" << std::endl;
        return false;
    }
    if (dest != '-' && ((activeColor == "w" && islower(dest)) || (activeColor == "b" && isupper(dest)))) 
    {
        std::cout << "King cannot move into piece of own color" << std::endl;
        return false;
    }


    bool kingUnderAttack = underAttack(toRow, toCol, activeColor, board, {});
    std::cout << "kingUnderAttack = " << kingUnderAttack << std::endl;
    // Check king cannot move into check
    if (kingUnderAttack)
    {
        std::cout << "King cannot move into check" << std::endl;
        RCLCPP_DEBUG(logger, "King cannot move into check");
        std::cout << "King cannot move into check" << std::endl;
        return false;
    }

    return true;
}

bool MoveValidator::underAttack(int row, int col, string &activeColor, const vector<vector<char>> &board, const vector<string> &moveHistory)
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
                        std::cout << "Square is under attack by piece at " << i << j << std::endl;
                        std::cout << piece << std::endl;

                        std::cout << board[i][j] << std::endl;
                        std::cout << board[0][0] << std::endl;
                        std::cout << board[0][1] << std::endl;

                        // Print the board for debugging                        
                        for (int i = 0; i < 8; i++)
                        {
                            for (int j = 0; j < 8; j++)
                            {
                                std::cout << board[i][j] << " ";
                            }
                            std::cout << std::endl;
                        }

                        return true; // The square is under attack
                    }
                }
            }
        }
    }
    std::cout << "Square is not under attack" << std::endl;
    return false;
}