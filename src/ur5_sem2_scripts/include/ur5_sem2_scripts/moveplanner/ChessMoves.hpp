#pragma once

#include "ur5_sem2_scripts/moveStruct.hpp"
#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.h>
#include <tf2/LinearMath/Quaternion.h>
#include <geometry_msgs/msg/pose.h>
#include <moveit_msgs/msg/joint_constraint.h>
#include <string>
#include <vector>
#include <array>

class ChessMoves {
public:
    explicit ChessMoves(const rclcpp::Node::SharedPtr& node);
    ~ChessMoves();
    bool move_to_idle();
    bool move(MoveStruct move, double TFchess[4][4]);
    bool remove_piece(MoveStruct move, double TFchess[4][4]);
    bool add_piece(MoveStruct move, double TFchess[4][4]);
    bool move_piece(MoveStruct move, double TFchess[4][4]);
    bool capture_piece(MoveStruct move, double TFchess[4][4]);
    bool promote_pawn(MoveStruct move, double TFchess[4][4]);
    bool castle(MoveStruct move, double TFchess[4][4]);
    bool en_passant(MoveStruct move, double TFchess[4][4]);
    bool playercapture(MoveStruct move, double TFchess[4][4]);
    bool execute_move(std::array<double, 2> start, std::array<double, 2> end, std::array<bool, 2> boardheight = {{false, false}});

    std::array<double, 2> applyTransformation(int point[2], double TFchess[4][4]);

    const std::array<std::array<double, 4>, 4> TFRed = 
        {{{1.0, 0.0, 0.0, 90.0},
        {0.0, 1.0, 0.0, -30.0},
        {0.0, 0.0, 1.0, 0.0},
        {0.0, 0.0, 0.0, 1.0}}};
    const std::array<std::array<double, 4>, 4> TFYellow = 
        {{{1.0, 0.0, 0.0, 35.0},
        {0.0, 1.0, 0.0, 25.0},
        {0.0, 0.0, 1.0, 0.0},
        {0.0, 0.0, 0.0, 1.0}}};

private:
    rclcpp::Node::SharedPtr node_;  // Store node reference
    moveit::planning_interface::MoveGroupInterface move_group_interface;  // MoveGroupInterface requires initialization
    const float TRANSFERHEIGHT = 0.35;  // Height to lift piece to transfer
    const float BOARDHEIGHT = 0.2;  // Height to set piece down on board
    const float TABLEHEIGHT = 0.1; // Height of the table

    const float idle_position[3] = {0.15, 0.0, TRANSFERHEIGHT};  // Idle position of the robot
    const float tile_size = 0.0353;  // Size of each square on the chess board

    


    const float death_positionsleft[16][2] = {
        {0.45, -0.30}, {0.45, -0.25}, {0.50, -0.30}, {0.50, -0.25},
        {0.55, -0.30}, {0.55, -0.25}, {0.60, -0.30}, {0.60, -0.25},
        {0.65, -0.30}, {0.65, -0.25}, {0.70, -0.30}, {0.70, -0.25},
        {0.75, -0.30}, {0.75, -0.25}, {0.80, -0.30}, {0.80, -0.25}
    };
    const float death_positionsright[16][2] = {
        {0.45, 0.25}, {0.45, 0.20}, {0.50, 0.25}, {0.50, 0.20},
        {0.55, 0.25}, {0.55, 0.20}, {0.60, 0.25}, {0.60, 0.20},
        {0.65, 0.25}, {0.65, 0.20}, {0.70, 0.25}, {0.70, 0.20},
        {0.75, 0.25}, {0.75, 0.20}, {0.80, 0.25}, {0.80, 0.20}
    };
    const float player_capture_position[2] = {0.375, -0.025};  // Position to place captured piece

    char deadPiecesLeft[16] = {'-','-','-','-','-','-','-','-','-', '-', '-', '-', '-', '-', '-', '-'};
    char deadPiecesRight[16] = {'-','-','-','-','-','-','-','-','-', '-', '-', '-', '-', '-', '-', '-'};

};
