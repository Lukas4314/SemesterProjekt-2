#include <cstdint>  // For uint8_t



class PicoCommunicator
{
public:
    PicoCommunicator();
    ~PicoCommunicator();

    void sendByte(uint8_t byte);
    int receiveByte();

private:
    int serialPort = -1;

};

