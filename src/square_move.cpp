#include <memory>

#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.h>
#include <tf2/LinearMath/Quaternion.h>
#include <geometry_msgs/msg/pose.h>
#include <moveit_msgs/msg/joint_constraint.h>

int main(int argc, char *argv[])
{
  // Initialize ROS and create the Node
  rclcpp::init(argc, argv);
  auto const node = std::make_shared<rclcpp::Node>(
      "hello_moveit",
      rclcpp::NodeOptions().automatically_declare_parameters_from_overrides(true));

  // Create a ROS logger
  auto const logger = rclcpp::get_logger("hello_moveit");




  
  // Create the MoveIt MoveGroup Interface
  using moveit::planning_interface::MoveGroupInterface;
  auto move_group_interface = MoveGroupInterface(node, "ur_manipulator");

  move_group_interface.setPlanningTime(45.0);
  move_group_interface.setNumPlanningAttempts(10);
  move_group_interface.setPoseReferenceFrame("chess_frame");

  // Define waypoints for a square path
  std::vector<geometry_msgs::msg::Pose> waypoints;

  geometry_msgs::msg::Pose start_pose;
  start_pose.orientation.w = 1.0; // Neutral orientation
  start_pose.position.x = 0.0;
  start_pose.position.y = 0.0;
  start_pose.position.z = 0.05;

  // Set orientation using a quaternion
  tf2::Quaternion quat;
  quat.setRPY(0, M_PI, 0);
  start_pose.orientation.x = quat.x();
  start_pose.orientation.y = quat.y();
  start_pose.orientation.z = quat.z();
  start_pose.orientation.w = quat.w();

  move_group_interface.setPoseTarget(start_pose);

  // Create a plan to that target pose
  auto const [success, plan] = [&move_group_interface]{
    moveit::planning_interface::MoveGroupInterface::Plan msg;
    auto const ok = static_cast<bool>(move_group_interface.plan(msg));
    return std::make_pair(ok, msg);
  }();

  // Execute the plan
  if(success) {
    move_group_interface.execute(plan);
  } else {
    RCLCPP_ERROR(logger, "Planing failed!");
  }

  // Add waypoints for the square
  waypoints.push_back(start_pose);

  geometry_msgs::msg::Pose pose1 = start_pose;
  pose1.position.y = 0.30;
  waypoints.push_back(pose1);

  geometry_msgs::msg::Pose pose2 = pose1;
  pose2.position.x = 0.40;
  waypoints.push_back(pose2);

  geometry_msgs::msg::Pose pose3 = pose2;
  pose3.position.z = 0.45;
  waypoints.push_back(pose3);

  geometry_msgs::msg::Pose pose4 = pose3;
  pose4.position.y = 0.0;
  waypoints.push_back(pose4);

  geometry_msgs::msg::Pose pose5 = pose4;
  pose5.position.z = 0.05;
  waypoints.push_back(pose5);

  geometry_msgs::msg::Pose pose6 = pose5;
  pose6.position.x = 0.0;
  waypoints.push_back(pose6); // Return to start position

  // Plan Cartesian path
  moveit_msgs::msg::RobotTrajectory trajectory;
  const double jump_threshold = 0.0; // No jump threshold for now
  const double eef_step = 0.01;       // Resolution of trajectory
  double fraction = move_group_interface.computeCartesianPath(waypoints, eef_step, jump_threshold, trajectory);

  rclcpp::sleep_for(std::chrono::seconds(3));
  if (fraction > 0.99)
  {
    RCLCPP_INFO(logger, "Executing Cartesian path");
    move_group_interface.execute(trajectory);
  }
  else
  {
    RCLCPP_ERROR(logger, "Cartesian path planning failed with %f success rate", fraction);
  }

  // Shutdown ROS
  rclcpp::shutdown();
  return 0;
}
