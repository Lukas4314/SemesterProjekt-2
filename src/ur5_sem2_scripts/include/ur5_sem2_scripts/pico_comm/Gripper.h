#pragma once
#include <cstdint>
#include <rclcpp/rclcpp.hpp>
#include <string>

class Gripper
{
public:
    Gripper();
    ~Gripper();

    bool openGripper();
    bool closeGripper();

    void sendByte(uint8_t byte);
    int receiveByte();
    void sendCommand(uint8_t command);
    std::string translateCodeToString(int code);

private:
    int serialPort = -1;
    rclcpp::Logger logger_ = rclcpp::get_logger("Gripper");
};
