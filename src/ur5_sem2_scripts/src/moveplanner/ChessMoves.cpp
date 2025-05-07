
#include "ur5_sem2_scripts/moveplanner/ChessMoves.hpp"



ChessMoves::ChessMoves(const rclcpp::Node::SharedPtr& node)
    : node_(node), move_group_interface(node, "ur_manipulator") { // Adjust "ur_manipulator" as needed
    RCLCPP_INFO(node_->get_logger(), "ChessMoves class initialized");

    move_group_interface.setPlanningTime(45.0);
    move_group_interface.setNumPlanningAttempts(10);
    move_group_interface.setPoseReferenceFrame("svejse_frame");
    move_group_interface.startStateMonitor(5.0);  // Increase timeout
    move_group_interface.setMaxVelocityScalingFactor(0.1);
    move_group_interface.setMaxAccelerationScalingFactor(0.1);
    move_group_interface.setStartStateToCurrentState();
    
}

ChessMoves::~ChessMoves() {
    RCLCPP_INFO(node_->get_logger(), "ChessMoves class destroyed");
}

bool ChessMoves::move_to_idle() {
    RCLCPP_INFO(node_->get_logger(), "move_to_idle() called");
    move_group_interface.setStartStateToCurrentState();
    std::map<std::string, double> target_joints;
    target_joints["shoulder_pan_joint"] = -21.0 * M_PI / 180.0;
    target_joints["shoulder_lift_joint"] = -31.0 * M_PI / 180.0;
    target_joints["elbow_joint"] = -131.0 * M_PI / 180.0;
    target_joints["wrist_1_joint"] = -108.0 * M_PI / 180.0;
    target_joints["wrist_2_joint"] = 90.0 * M_PI / 180.0;
    target_joints["wrist_3_joint"] = 137.0 * M_PI / 180.0;
    move_group_interface.setJointValueTarget(target_joints);
    move_group_interface.move();
    return true;
}

bool ChessMoves::move(moveStruct move, double TFchess[4][4]) {
    RCLCPP_INFO(node_->get_logger(), "move() called");
    switch (move.type) {
        case 'm':

            if (move.captured != '-') {
                return capture_piece(move, TFchess);
            } else {
                return move_piece(move, TFchess);
            }
        case 'r':
            return remove_piece(move, TFchess);
        case 'a':
            return add_piece(move, TFchess);
        case 'p':
            return promote_pawn(move, TFchess);
        case 'c':
            return castle(move, TFchess);
        case 'e':
            return en_passant(move, TFchess);
        case 'k':
            return playercapture(move, TFchess);
        default:
            return false;
    }
}

bool ChessMoves::remove_piece(moveStruct move, double TFchess[4][4]) {
    RCLCPP_INFO(node_->get_logger(), "remove_piece() called");
    double deathposition[2] = {0, 0};
    if (move.color == 'w') {
        for (int i = 0; i < 16; i++) {
            if (deadPiecesLeft[i] == '-') {
                deadPiecesLeft[i] = move.piece;
                deathposition[0] = death_positionsleft[i][0];
                deathposition[1] = death_positionsleft[i][1];
                break;
            }
        }

    } else {
        for (int i = 0; i < 16; i++) {
            if (deadPiecesRight[i] == '-') {
                deadPiecesRight[i] = move.piece;
                deathposition[0] = death_positionsright[i][0];
                deathposition[1] = death_positionsright[i][1];
                break;
            }
        }
    }
    // apply transformation to start position
    std::array<double, 2> start = applyTransformation(move.start, TFchess);
    std::array<double, 2> end = {deathposition[0],deathposition[1]};
    
    RCLCPP_INFO(node_->get_logger(), "start: %f, %f", start[0], start[1]);
    // Execute the move
    if(execute_move(start, end))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool ChessMoves::add_piece(moveStruct move, double TFchess[4][4]) {
    RCLCPP_INFO(node_->get_logger(), "add_piece() called");
    double deathposition[2] = {0, 0};

    //output the dead pieces
    RCLCPP_INFO(node_->get_logger(), "dead pieces left: ");
    for (int i = 0; i < 16; i++) {
        RCLCPP_INFO(node_->get_logger(), "%c", deadPiecesLeft[i]);
    }
    RCLCPP_INFO(node_->get_logger(), "dead pieces right: ");
    for (int i = 0; i < 16; i++) {
        RCLCPP_INFO(node_->get_logger(), "%c", deadPiecesRight[i]);
    }

    if (move.color == 'w') {
        for (int i = 0; i < 16; i++) {
            if (toupper(deadPiecesLeft[i]) == toupper(move.piece)) {
                deadPiecesLeft[i] = '-';
                deathposition[0] = death_positionsleft[i][0];
                deathposition[1] = death_positionsleft[i][1];
                RCLCPP_INFO(node_->get_logger(), "dead piece: %c", deadPiecesLeft[i]);
                RCLCPP_INFO(node_->get_logger(), "deathposition: %f, %f", deathposition[0], deathposition[1]);
                break;
            }
        }

    } else {
        for (int i = 0; i < 16; i++) {
            if (toupper(deadPiecesRight[i]) == toupper(move.piece)) {
                deadPiecesRight[i] = '-';
                deathposition[0] = death_positionsright[i][0];
                deathposition[1] = death_positionsright[i][1];
                RCLCPP_INFO(node_->get_logger(), "dead piece: %c", deadPiecesRight[i]);
                RCLCPP_INFO(node_->get_logger(), "deathposition: %f, %f", deathposition[0], deathposition[1]);
                break;
            }
        }
    }
    std::array<double, 2> start = {deathposition[0], deathposition[1]};
    std::array<double, 2> end = applyTransformation(move.end, TFchess);

    RCLCPP_INFO(node_->get_logger(), "start: %f, %f", start[0], start[1]);
    if(execute_move(start, end))
    {
        return true;
    }
    else
    {
        return false;
    }




}

bool ChessMoves::move_piece(moveStruct move, double TFchess[4][4]) {
    RCLCPP_INFO(node_->get_logger(), "move_piece() called");
    // apply transformation to start position
    std::array<double, 2> start = applyTransformation(move.start, TFchess);
    // apply transformation to end position
    std::array<double, 2> end = applyTransformation(move.end, TFchess);


    if (execute_move(start, end)) {
        return true;
    } else {
        return false;
    }


}

bool ChessMoves::capture_piece(moveStruct move, double TFchess[4][4]) {
    RCLCPP_INFO(node_->get_logger(), "capture_piece() called");
    moveStruct removeMove;
    removeMove.piece = move.captured;
    if (move.color == 'w') {
        removeMove.color = 'b';
    } else {
        removeMove.color = 'w';
    }
    removeMove.start[0] = move.end[0];
    removeMove.start[1] = move.end[1];
    removeMove.end[0] = 0;
    removeMove.end[1] = 0;
    removeMove.type = 'r';
    remove_piece(removeMove, TFchess);
    move_piece(move, TFchess);


    return true;
}

bool ChessMoves::promote_pawn(moveStruct move, double TFchess[4][4]) {
    RCLCPP_INFO(node_->get_logger(), "promote_pawn() called");

    if (move.captured != '-') {
        moveStruct removeMove;
        removeMove.piece = move.captured;
        if (move.color == 'w') {
            removeMove.color = 'b';
        } else {
            removeMove.color = 'w';
        }
        removeMove.start[0] = move.end[0];
        removeMove.start[1] = move.end[1];
        removeMove.end[0] = 0;
        removeMove.end[1] = 0;
        removeMove.type = 'r';
        remove_piece(removeMove, TFchess);
    }

    
    moveStruct removeMove = move;
    removeMove.type = 'r';
    remove_piece(removeMove, TFchess);

    moveStruct addMove = move;
    addMove.type = 'a';
    addMove.piece = move.promotion;
    add_piece(addMove, TFchess);




    return true;
}

bool ChessMoves::castle(moveStruct move, double TFchess[4][4]) {
    RCLCPP_INFO(node_->get_logger(), "castle() called");

    if (move.end[1] == 2) {
        moveStruct rookMove;
        rookMove.piece = 'r';
        rookMove.color = move.color;
        rookMove.start[0] = move.start[0];
        rookMove.start[1] = 0;
        rookMove.end[0] = move.start[0];
        rookMove.end[1] = 3;
        rookMove.type = 'm';
        move_piece(rookMove, TFchess);
        moveStruct kingMove;
        kingMove.piece = 'k';
        kingMove.color = move.color;
        kingMove.start[0] = move.start[0];
        kingMove.start[1] = 4;
        kingMove.end[0] = move.start[0];
        kingMove.end[1] = 2;
        kingMove.type = 'm';
        move_piece(kingMove, TFchess);
    } else {
        moveStruct rookMove;
        rookMove.piece = 'r';
        rookMove.color = move.color;
        rookMove.start[0] = move.start[0];
        rookMove.start[1] = 7;
        rookMove.end[0] = move.start[0];
        rookMove.end[1] = 5;
        rookMove.type = 'm';
        move_piece(rookMove, TFchess);
        moveStruct kingMove;
        kingMove.piece = 'k';
        kingMove.color = move.color;
        kingMove.start[0] = move.start[0];
        kingMove.start[1] = 4;
        kingMove.end[0] = move.start[0];
        kingMove.end[1] = 6;
        kingMove.type = 'm';
        move_piece(kingMove, TFchess);
    }

    return true;
}

bool ChessMoves::en_passant(moveStruct move, double TFchess[4][4]) {
    RCLCPP_INFO(node_->get_logger(), "en_passant() called");

    moveStruct removeMove;
    removeMove.piece = 'p';
    if (move.color == 'w') {
        removeMove.color = 'b';
    } else {
        removeMove.color = 'w';
    }
    removeMove.start[0] = move.start[0];
    removeMove.start[1] = move.end[1];
    removeMove.end[0] = 0;
    removeMove.end[1] = 0;
    removeMove.type = 'r';
    remove_piece(removeMove, TFchess);
    move_piece(move, TFchess);



    return true;
}

bool ChessMoves::playercapture(moveStruct move, double TFchess[4][4]) {
    RCLCPP_INFO(node_->get_logger(), "playercapture() called");
    double deathposition[2] = {0, 0};
    if (move.color == 'w') {
        for (int i = 0; i < 16; i++) {
            if (deadPiecesLeft[i] == '-') {
                deadPiecesLeft[i] = move.piece;
                deathposition[0] = death_positionsleft[i][0];
                deathposition[1] = death_positionsleft[i][1];
                break;
            }
        }

    } else {
        for (int i = 0; i < 16; i++) {
            if (deadPiecesRight[i] == '-') {
                deadPiecesRight[i] = move.piece;
                deathposition[0] = death_positionsright[i][0];
                deathposition[1] = death_positionsright[i][1];
                break;
            }
        }
    }
    
    // set start position to player capture position
    // apply transformation to start position
    std::array<double, 2> start = {player_capture_position[0], player_capture_position[1]};
    std::array<double, 2> end = {deathposition[0], deathposition[1]};

    execute_move(start, end);


    return true;
}

bool ChessMoves::execute_move(std::array<double, 2> start, std::array<double, 2> end) {
    RCLCPP_INFO(node_->get_logger(), "execute_move() called");

    
    std::vector<geometry_msgs::msg::Pose> waypoints;

    geometry_msgs::msg::Pose start_pose;
    tf2::Quaternion quat;
    quat.setRPY(0, M_PI, 0);
    start_pose.orientation.x = quat.x();
    start_pose.orientation.y = quat.y();
    start_pose.orientation.z = quat.z();
    start_pose.orientation.w = quat.w();
    start_pose.position.x = idle_position[0];
    start_pose.position.y = idle_position[1];
    start_pose.position.z = idle_position[2];


    waypoints.push_back(start_pose);
    geometry_msgs::msg::Pose pose1;
    // Set orientation using a quaternion
    quat.setRPY(0, M_PI, 0);
    pose1.orientation.x = quat.x();
    pose1.orientation.y = quat.y();
    pose1.orientation.z = quat.z();
    pose1.orientation.w = quat.w();
    pose1.position.x = start[0];
    pose1.position.y = start[1];
    pose1.position.z = TRANSFERHEIGHT;
    waypoints.push_back(pose1);

    geometry_msgs::msg::Pose pose2 = pose1;
    pose2.position.z = BOARDHEIGHT;
    waypoints.push_back(pose2);
    
    // Plan Cartesian path for the first part of the move
    moveit_msgs::msg::RobotTrajectory trajectory;
    const double jump_threshold = 0.0; // No jump threshold for now
    const double eef_step = 0.01;       // Resolution of trajectory
    double fraction = move_group_interface.computeCartesianPath(waypoints, eef_step, jump_threshold, trajectory);

    rclcpp::sleep_for(std::chrono::seconds(1));
    if (fraction > 0.99)
    {
        RCLCPP_INFO(node_->get_logger(), "Executing Cartesian path");
        move_group_interface.execute(trajectory);
    }
    else
    {
        RCLCPP_ERROR(node_->get_logger(), "Cartesian path planning failed with %f success rate", fraction);
    }

    // Pick up the piece
    // gripper.closeGripper();
    rclcpp::sleep_for(std::chrono::seconds(0));

    // Move to the end position
    waypoints.clear();
    geometry_msgs::msg::Pose pose3 = pose1;
    pose3.position.x = end[0];
    pose3.position.y = end[1];
    waypoints.push_back(pose2);
    waypoints.push_back(pose1);
    waypoints.push_back(pose3);

    geometry_msgs::msg::Pose pose4 = pose3;
    pose4.position.z = BOARDHEIGHT;
    waypoints.push_back(pose4);

    // Plan Cartesian path for the piece transfer
    moveit_msgs::msg::RobotTrajectory trajectory1;
    fraction = move_group_interface.computeCartesianPath(waypoints, eef_step, jump_threshold, trajectory1);

    rclcpp::sleep_for(std::chrono::seconds(1));
    if (fraction > 0.99)
    {
        RCLCPP_INFO(node_->get_logger(), "Executing Cartesian path");
        move_group_interface.execute(trajectory1);
    }
    
    else
    {
        RCLCPP_ERROR(node_->get_logger(), "Cartesian path planning failed with %f success rate", fraction);
    }

    // Set the piece down
    // gripper.openGripper();

    rclcpp::sleep_for(std::chrono::seconds(0));


    // Move to idle position
    
    waypoints.clear();

    geometry_msgs::msg::Pose pose5 = pose4;
    waypoints.push_back(pose5);

    geometry_msgs::msg::Pose pose6 = pose3;
    pose6.position.z = TRANSFERHEIGHT;
    waypoints.push_back(pose6);

    geometry_msgs::msg::Pose pose7 = pose6;
    pose7.position.x = idle_position[0];
    pose7.position.y = idle_position[1];
    pose7.position.z = idle_position[2];
    waypoints.push_back(pose7);
    // Plan Cartesian path for the move to idle position
    moveit_msgs::msg::RobotTrajectory trajectory2;
    fraction = move_group_interface.computeCartesianPath(waypoints, eef_step, jump_threshold, trajectory2);

    rclcpp::sleep_for(std::chrono::seconds(1));
    if (fraction > 0.99)
    {
        RCLCPP_INFO(node_->get_logger(), "Executing Cartesian path");
        move_group_interface.execute(trajectory2);
    }
    else
    {
        RCLCPP_ERROR(node_->get_logger(), "Cartesian path planning failed with %f success rate", fraction);
    }


    return true;
}

std::array<double, 2> ChessMoves::applyTransformation(int point[2], double TFchess[4][4]) {
    RCLCPP_INFO(node_->get_logger(), "applyTransformation() called");
    std::array<double, 2> transformed_point;
    transformed_point[0] = TFchess[0][0] * (point[0] * tile_size + 0.024) + TFchess[0][1] * (point[1] * tile_size + 0.024) + TFchess[0][3];
    transformed_point[1] = TFchess[1][0] * (point[0] * tile_size + 0.024) + TFchess[1][1] * (point[1] * tile_size + 0.024) + TFchess[1][3];
    RCLCPP_INFO(node_->get_logger(), "Original point: (%d, %d)", point[0], point[1]);
    RCLCPP_INFO(node_->get_logger(), "TF values: (%f, %f, %f, %f)", TFchess[0][0], TFchess[0][1], TFchess[0][3], TFchess[1][3]);
    RCLCPP_INFO(node_->get_logger(), "Transformed point: (%f, %f)", transformed_point[0], transformed_point[1]);
    return transformed_point;
}