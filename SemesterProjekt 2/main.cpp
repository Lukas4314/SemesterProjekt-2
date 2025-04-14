#include <iostream>
#include "picoCommunicator.h"
#include <chrono>
#include <thread>

int main()
{
	PicoCommunicator communicator;
	uint8_t count = 1;
	while (true)
	{
		communicator.sendByte(count);			   // Send an integer to the Pico
		std::cout << "Sent: " << (int)count << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));


		int received = communicator.receiveByte(); // Receive an integer from the Pico
		std::cout << "Received: " << received << std::endl;

		count = received + 1;
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	return 0;
}