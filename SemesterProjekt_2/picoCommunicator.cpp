#include "picoCommunicator.h"
#include <string>
#include <fcntl.h>   // open
#include <errno.h>   // errno
#include <termios.h> // termios
#include <unistd.h>  // write, read, close
#include <iostream>
#include <cstring> // std::strerror
#include <cstdint> // uint8_t

#define SERIAL_PORT "/dev/ttyACM0"
#define BAUD_RATE B115200

PicoCommunicator::PicoCommunicator()
{
    serialPort = open(SERIAL_PORT, O_RDWR | O_NOCTTY | O_NDELAY);
    if (serialPort == -1)
    {
        std::cerr << "Failed to open serial port: " << std::strerror(errno) << std::endl;
        return;
    }

    struct termios tty;
    if (tcgetattr(serialPort, &tty) != 0)
    {
        std::cerr << "Error getting terminal attributes: " << std::strerror(errno) << std::endl;
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

PicoCommunicator::~PicoCommunicator()
{
    if (serialPort != -1)
    {
        close(serialPort);
    }
}

void PicoCommunicator::sendByte(uint8_t byte)
{
    if (serialPort == -1) {
        return;
    }

    ssize_t written = write(serialPort, &byte, 1); // Send only 1 byte
    if (written < 0)
    {
        std::cerr << "Failed to write to serial port: " << std::strerror(errno) << std::endl;
    }
}

int PicoCommunicator::receiveByte()
{
    if (serialPort == -1)
        return -1;

    uint8_t buffer[1];  // Buffer for 1 byte
    ssize_t bytesRead = read(serialPort, buffer, 1); // Read 1 byte

    if (bytesRead != 1)
    {
        std::cerr << "Failed to read 1 byte from serial port: " << std::strerror(errno) << std::endl;
        return -1;
    }
    
    return buffer[0]; // Return the received byte
}
