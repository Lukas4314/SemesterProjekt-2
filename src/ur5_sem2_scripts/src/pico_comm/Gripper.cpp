#include "Gripper.h"
#include <string>
#include <fcntl.h>   // open
#include <errno.h>   // errno
#include <termios.h> // termios
#include <unistd.h>  // write, read, close
#include <rclcpp/rclcpp.hpp>
#include <cstring> // std::strerror
#include <cstdint> // uint8_t

#define SERIAL_PORT "/dev/ttyACM0"
#define BAUD_RATE B115200
#define CLOSE_GRIPPER 0x01
#define OPEN_GRIPPER 0x02
#define GRIPPER_STATUS 0x03
#define GRIPPER_OPENING 0x13
#define GRIPPER_CLOSING 0x14
#define GRIPPER_ERROR 0x04
#define GRIPPER_ACK 0x06
#define GRIPPER_NACK 0x15
#define GRIPPER_OPEN 0x07
#define GRIPPER_CLOSED 0x08
#define GETADC 0x09

#define GRIPPER_TIMEOUT 1000 // milliseconds

Gripper::Gripper()
    : logger_(rclcpp::get_logger("Gripper"))
{
    serialPort = open(SERIAL_PORT, O_RDWR | O_NOCTTY | O_NDELAY);
    if (serialPort == -1)
    {
        RCLCPP_ERROR(logger_, "Failed to open serial port: %s", std::strerror(errno));
        return;
    }

    struct termios tty;
    if (tcgetattr(serialPort, &tty) != 0)
    {
        RCLCPP_ERROR(logger_, "Error getting terminal attributes: %s", std::strerror(errno));
        close(serialPort);
        serialPort = -1;
        return;
    }

    cfsetispeed(&tty, BAUD_RATE);
    cfsetospeed(&tty, BAUD_RATE);

    tty.c_cflag |= (CLOCAL | CREAD); // Enable receiver, ignore modem control lines
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;      // 8 data bits
    tty.c_cflag &= ~PARENB;  // No parity
    tty.c_cflag &= ~CSTOPB;  // 1 stop bit
    tty.c_cflag &= ~CRTSCTS; // No hardware flow control

    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // Raw input
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);         // No software flow control
    tty.c_oflag &= ~OPOST;                          // Raw output

    tcsetattr(serialPort, TCSANOW, &tty);
}

Gripper::~Gripper()
{
    if (serialPort != -1)
    {
        close(serialPort);
    }
}

void Gripper::sendByte(uint8_t byte)
{
    if (serialPort != -1)
    {
        write(serialPort, &byte, sizeof(byte));
    }
}

int Gripper::receiveByte()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Wait for 100ms before reading
    if (serialPort == -1)
    {
        std::cout << "Serial port not open" << std::endl;
        return -1;
    }

    uint8_t buffer[1]; // Buffer for 1 byte
    int timeTillTimeout = 5000;
    while (true)
    {
        ssize_t bytesRead = read(serialPort, buffer, 1); // Read 1 byte

        if (bytesRead == 1)
        {
            break;
        }

        RCLCPP_DEBUG(logger_, "Failed to read 1 byte from serial port: %s", std::strerror(errno));
        timeTillTimeout -= 100;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (timeTillTimeout <= 0)
        {
            RCLCPP_ERROR(logger_, "Timeout while waiting for response from gripper");
            return -1; // Timeout
        }
    }
    std::cout << "Received byte: " << static_cast<int>(buffer[0]) << std::endl;
    return buffer[0]; // Return the received byte
}

void Gripper::sendCommand(uint8_t command)
{
    sendByte(command);
    int response = receiveByte();
    std::cout << "Received command: " << translateCodeToString(response) << std::endl;
    if (response == GRIPPER_ACK)
    {
        RCLCPP_DEBUG(logger_, "Command acknowledged");
    }
    else if (response == GRIPPER_NACK)
    {
        RCLCPP_ERROR(logger_, "Command not acknowledged");
    }
    else
    {
        RCLCPP_DEBUG(logger_, "Unexpected response: %d", response);
    }
}

bool Gripper::openGripper()
{
    sendCommand(OPEN_GRIPPER);

    while (true)
    {
        sendCommand(GRIPPER_STATUS);
        int status = receiveByte();
        if (status == GRIPPER_OPENING)
        {
            RCLCPP_INFO(logger_, "Gripper is opening");
        }
        else if (status == GRIPPER_OPEN)
        {
            RCLCPP_DEBUG(logger_, "Gripper opened successfully");
            return true;
        }
        else if (status == GRIPPER_ERROR)
        {
            RCLCPP_ERROR(logger_, "Gripper error occurred");
            return false;
        }
        else
        {
            RCLCPP_DEBUG(logger_, "Unexpected status: %d", status);
            return false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

bool Gripper::closeGripper()
{
    std::cout << "sending close command" << std::endl;
    sendCommand(CLOSE_GRIPPER);

    while (true)
    {
        std::cout << "Sending get gripper status command" << std::endl;
        sendCommand(GRIPPER_STATUS);
        int status = receiveByte();
        if (status == GRIPPER_CLOSING)
        {
            std::cout << "Gripper is closing" << std::endl;
            RCLCPP_INFO(logger_, "Gripper is closing");

            std::cout << "Sending get ADC command" << std::endl;
            sendCommand(GETADC);
            int adcValue = receiveByte() * 5;
            std::cout << "ADC Value: " << adcValue << std::endl;
        }
        else if (status == GRIPPER_CLOSED)
        {
            std::cout << "Gripper closed succesfully" << std::endl;
            RCLCPP_DEBUG(logger_, "Gripper closed successfully");
            return true;
        }
        else if (status == GRIPPER_ERROR)
        {
            std::cout << "Gripper error occurred" << std::endl;
            RCLCPP_ERROR(logger_, "Gripper error occurred");
            return false;
        }
        else
        {
            std::cout << "Unexpected status: " << status << std::endl;
            RCLCPP_DEBUG(logger_, "Unexpected status: %d", status);
            return false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

std::string Gripper::translateCodeToString(int code)
{
    if (code == GRIPPER_ACK)
    {
        return "ACK";
    }
    else if (code == GRIPPER_NACK)
    {
        return "NACK";
    }
    else if (code == GRIPPER_OPEN)
    {
        return "OPEN";
    }
    else if (code == GRIPPER_CLOSED)
    {
        return "CLOSED";
    }
    else if (code == GRIPPER_OPENING)
    {
        return "OPENING";
    }
    else if (code == GRIPPER_CLOSING)
    {
        return "CLOSING";
    }
    else if (code == GRIPPER_ERROR)
    {
        return "ERROR";
    }
    else
    {
        return "UNKNOWN CODE: " + std::to_string(code);
    }
}