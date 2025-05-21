#include "ur5_sem2_scripts/logger/Logger.h"



int main() {

    Logger::initialize(Logger::getAllLoggerKeys());

    Logger::setValue(GRIPPER_PICKUP_OWN_PIECE_SUCCESS, "0");
    Logger::setValue(GRIPPER_PUTDOWN_DEAD_PIECE_SUCCESS, "1");
    Logger::setValue(GRIPPER_SHOULD_PICKUP_DEAD_PIECE, "2");
    Logger::setValue(GRIPPER_PICKUP_DEAD_PIECE_SUCCESS, "1");
    Logger::setValue(GRIPPER_PICKUP_DEAD_PIECE_SUCCESS, "1");

    Logger::writeRow();

    return 0;
}

