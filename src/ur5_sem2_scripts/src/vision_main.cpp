#include "iostream"
#include "string"
#include "AllInOneMain.h"
#include "opencv2/opencv.hpp"
#include <string>
#include "ChessBoard.h"
#include "StockfishUCI.h"
#include "ament_index_cpp/get_package_share_directory.hpp"
#include "Utill.h"

using namespace std;

int main(int argc, char *argv[])
{
    string activeColor = "w"; // White starts
    string package_share_dir = ament_index_cpp::get_package_share_directory("ur5_sem2_scripts");
    // Initialize RCLCPP
    rclcpp::init(argc, argv);
    rclcpp::Logger const logger = rclcpp::get_logger("vision_main");
    auto const node = make_shared<rclcpp::Node>(
        "vision_main",
        rclcpp::NodeOptions().automatically_declare_parameters_from_overrides(true));

    RCLCPP_INFO(logger, "Starting chess program");
    RCLCPP_DEBUG(logger, "Debugging started");
    int camera_index;
    node->get_parameter("camera_index", camera_index); // Retrieve the parameter
    ChessBoard chess;
    StockfishUCI engine;
    int moveCounter = 0;

    cout << "Do you want to play as white or black? (Enter 'w' for white, 'b' for black): ";
    cin >> activeColor;

    if (activeColor != "w" && activeColor != "b") {
        cerr << "Invalid input! Defaulting to white." << endl;
        activeColor = "w";
    }

    AllInOneMain allInOneMain = AllInOneMain(camera_index);
    allInOneMain.getPieceMovedString(0);

    // If the user chooses black, Stockfish makes the first move
    if (activeColor == "b") {
        int depth = 15;
        vector<string> moveHistory = chess.getMoveHistory();
        string allMoves = Utill::vectorStringToString(moveHistory);
        string output = engine.getBestMove(allMoves, depth);

        size_t pos = output.find("bestmove ");
        if (pos == string::npos) {
            cerr << "Failed to find bestmove in engine output!" << endl;
            return -1;
        }

        size_t start = output.find("bestmove ") + 9;
        size_t end = output.find(' ', start);
        string bestMove = output.substr(start, end - start);

        RCLCPP_DEBUG(logger, ("Applying engine move"));
        RCLCPP_DEBUG(logger, ("Bestmove: " + bestMove).c_str());
        string translatedBestMove = Utill::translateEngineBestMove(bestMove);

        chess.applyIfValidMove(translatedBestMove);
        RCLCPP_DEBUG(logger, ("Engine Move is: " + translatedBestMove).c_str());
        chess.printBoard();
    }
    

    cv::waitKey(0);
    while (rclcpp::ok())
    {
        string move;
        bool succesMove;
        int moveDepth = 0;

        do
        {
            move = allInOneMain.getPieceMovedString(moveDepth);
            cout << "Move is: " << move << endl;
            succesMove = chess.applyIfValidMove(move);
            cout << "Succes move: " << succesMove << endl;
            moveDepth++;

        } while (!succesMove);

        cout << "Camera Move is: " << move << endl;
        chess.printBoard();

        if (chess.isMated(chess.getActiveColor()))
        {
            cout << "Checkmate! " << chess.getActiveColor() << " is mated!" << endl;
            break;
        }

        if (chess.isRemi(chess.getActiveColor()))
        {
            cout << "Remi! " << endl;
            break;
        }

        // --- StockfishUCI start;
        int depth = 15;
        vector<string> moveHistory = chess.getMoveHistory();
        string allMoves = Utill::vectorStringToString(moveHistory);
        string output = engine.getBestMove(allMoves, depth);

        size_t pos = output.find("bestmove ");

        if (pos == string::npos)
        {
            cerr << "Failed to find bestmove in engine output!" << endl;
            return -1;
        }

        size_t start = output.find("bestmove ") + 9;
        size_t end = output.find(' ', start);
        string bestMove = output.substr(start, end - start);

        RCLCPP_DEBUG(logger, ("Applying engine move"));
        RCLCPP_DEBUG(logger, ("Bestmove: " + bestMove).c_str());
        string translatedBestMove = Utill::translateEngineBestMove(bestMove);
        //--- StockfishUCI end

        chess.applyIfValidMove(translatedBestMove);
        RCLCPP_DEBUG(logger, ("Engine Move is: " + translatedBestMove).c_str());
        chess.printBoard();

        if (chess.isMated(chess.getActiveColor()))
        {
            cout << "Checkmate! " << chess.getActiveColor() << " is mated!" << endl;
            break;
        }

        // robot moves and makes it move
        cv::waitKey(0);

        moveCounter++;
        cout << "Move counter: " << moveCounter << endl;

        allInOneMain.getPieceMovedString(0);
        cout << "IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII" << endl;
        if (cv::waitKey(0) == 'q')
        {
            break;
        }
    }

    rclcpp::shutdown();
    return 0;
}