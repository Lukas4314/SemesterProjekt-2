#include "ur5_sem2_scripts/pico_comm/Gripper.h"

int main()
{

  Gripper gripper;

  while (true)
  {
    std::cout << "Closing gripper..." << std::endl;
    gripper.closeGripper();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Opening gripper..." << std::endl;
    gripper.openGripper();
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  return 0;
}