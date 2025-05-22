#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>

// Define your header constants here
constexpr const char *MOVE_COUNT = "MoveCount";
constexpr const char *CAMERA_MOVE = "CameraMove";
constexpr const char *MOVE = "Move";

// ALl the gripper related constants
constexpr const char *GRIPPER_PICKUP_OWN_PIECE_SUCCESS = "GripperPickupOwnPieceSuccess";
constexpr const char *GRIPPER_PUTDOWN_OWN_PIECE_SUCCESS = "GripperPickupOwnPieceSuccess";
constexpr const char *GRIPPER_SHOULD_PICKUP_DEAD_PIECE = "GripperShouldPickupDeadPiece";
constexpr const char *GRIPPER_PICKUP_DEAD_PIECE_SUCCESS = "GripperPickupDeadPieceSuccess";
constexpr const char *GRIPPER_PUTDOWN_DEAD_PIECE_SUCCESS = "GripperPutdownDeadPieceSuccess";
constexpr const char *GRIPPER_PICKUP_ENEMY_PIECE = "GripperPickupEnemyPiece";
constexpr const char *GRIPPER_PICKUP_ENEMY_PIECE_SUCCESS = "GripperPickupEnemyPieceSuccess";
constexpr const char *GRIPPER_PUTDOWN_ENEMY_PIECE_SUCCESS = "GripperPutdownEnemyPieceSuccess";

// In case of castling
constexpr const char *EXTRA_PICKUP_FOR_CASTLING = "ExtraPickupForCastling";
constexpr const char *EXTRA_PICKUP_FOR_CASTLING_SUCCESS = "ExtraPickupForCastlingSuccess";

constexpr const char *ANGLE_OF_TRANSFORMATION_MATRIX_CHESSBOARD = "AngleOfTransformationMatrixChessboard";
constexpr const char *ANGLE_OF_TRANSFORMATION_MATRIX_PEGS = "AngleOfTransformationMatrixPegs";
constexpr const char *MOVES_TRIED_BEFORE_SUCCESS = "MovesTriedBeforeSuccess";
constexpr const char *CameraCorrectMove = "CameraCorrectImage";
constexpr const char *CameraCorrectYellowPegLocation = "CameraCorrectYellowPegLocation";
constexpr const char *CameraCorrectRedPegLocation = "CameraCorrectRedPegLocation";
constexpr const char *CameraCorrectGreenCornerLocation = "CameraCorrectGreenCornorLocation";
constexpr const char *CameraCorrectYellowCornorLocation = "CameraCorrectYellowCornorLocation";
constexpr const char *MOST_LIKELY_CAMERA_MOVE = "MostLikelyCameraMove";
constexpr const char *SECOND_MOST_LIKELY_CAMERA_MOVE = "SecondMostLikelyCameraMove";
constexpr const char *THIRD_MOST_LIKELY_CAMERA_MOVE = "ThirdMostLikelyCameraMove";
constexpr const char *MOST_LIKELY_CAMERA_MOVE_SCORE = "MostLikelyCameraMoveScore";
constexpr const char *SECOND_MOST_LIKELY_CAMERA_MOVE_SCORE = "SecondMostLikelyCameraMoveScore";
constexpr const char *THIRD_MOST_LIKELY_CAMERA_MOVE_SCORE = "ThirdMostLikelyCameraMoveScore";
constexpr const char *ROBOT_MANAGES_TO_MAKE_MOVEMENT = "RobotManagesToMakeMovement";

class Logger
{
private:
    static std::ofstream logFile;
    static std::string fileName;
    static std::vector<std::string> buffer;
    static std::vector<std::string> headers;
    static std::unordered_map<std::string, size_t> headerIndexMap;

    static std::string currentDateTimeForFile();
    static std::string generateUniqueFileName();
    static std::string escapeCsv(const std::string &s);

    static void writeHeaders();

public:
    // Must call first with your desired header constants (array of const char*)
    static void initialize(const std::vector<const char *> &headerNames);

    // Set value by header constant (which is a const char* known at compile time)
    static void setValue(const char *headerName, const std::string &value);

    // Write the buffer row
    static void writeRow();

    static void setStandardValues();

    static std::vector<const char *> getAllLoggerKeys();

    // Optional cleanup
    static void close();
};
