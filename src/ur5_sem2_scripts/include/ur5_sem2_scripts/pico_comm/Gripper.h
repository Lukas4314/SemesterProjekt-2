#pragma once
#include <cstdint>

class Gripper
{
    public:
    Gripper();
    ~Gripper();

    bool openGripper();
    bool closeGripper();

    void sendByte(uint8_t byte);
    int receiveByte();

private:
    int serialPort = -1;
};

Gripper::Gripper(/* args */)
{
}

Gripper::~Gripper()
{
}
