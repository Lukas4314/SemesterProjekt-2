#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.h>

int main(int argc, char * argv[])
{
  // Initialize ROS and create the Node
  rclcpp::init(argc, argv);
  auto const node = std::make_shared<rclcpp::Node>(
    "read_joint_values",
    rclcpp::NodeOptions().automatically_declare_parameters_from_overrides(true)
  );

  // Create a ROS logger
  auto const logger = rclcpp::get_logger("read_joint_values");

  // Create the MoveIt MoveGroup Interface for UR5 manipulator
  using moveit::planning_interface::MoveGroupInterface;
  auto move_group_interface = MoveGroupInterface(node, "ur_manipulator");

  // Get current joint values from the MoveGroupInterface
  const auto& current_joint_values = move_group_interface.getCurrentJointValues();

  // Output the joint values to the console
  RCLCPP_INFO(logger, "Current Joint Values:");
  for (size_t i = 0; i < current_joint_values.size(); ++i)
  {
    RCLCPP_INFO(logger, "Joint %zu: %.4f", i, current_joint_values[i]);
  }

  // Shutdown ROS
  rclcpp::shutdown();
  return 0;
}
