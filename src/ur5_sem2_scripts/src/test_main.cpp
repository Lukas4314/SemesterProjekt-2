#include "ur5_sem2_scripts/Logger.h"



int main() {
    Logger::initialize({HALF_MOVES, CAMERA_MOVE, FPS});

    Logger::setValue(HALF_MOVES, "10");
    Logger::setValue(CAMERA_MOVE, "MoveUp");
    Logger::setValue(FPS, "60");
    Logger::writeRow();

    Logger::setValue(HALF_MOVES, "11");
    Logger::setValue(CAMERA_MOVE, "RotateLeft");
    Logger::setValue(FPS, "55");
    Logger::writeRow();

    Logger::setValue(FPS, "55");

    Logger::setValue(HALF_MOVES, "11");
    Logger::setValue(FPS, "55");
    Logger::writeRow();

    Logger::setValue(FPS, "55");
    Logger::setValue(HALF_MOVES, "11");
    Logger::setValue(CAMERA_MOVE, "RotateLeft");
    Logger::writeRow();

    Logger::close();

    return 0;
}
