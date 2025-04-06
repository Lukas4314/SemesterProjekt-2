#include <memory>

#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.h>
#include <tf2/LinearMath/Quaternion.h>
#include <geometry_msgs/msg/pose.h>
#include <moveit_msgs/msg/joint_constraint.h>
#include <string>

int main(int argc, char * argv[])
{
  // Initialize ROS and create the Node
  rclcpp::init(argc, argv);
  auto const node = std::make_shared<rclcpp::Node>(
    "hello_moveit",
    rclcpp::NodeOptions().automatically_declare_parameters_from_overrides(true)
  );

  // Create a ROS logger
  auto const logger = rclcpp::get_logger("hello_moveit");

  // Create the MoveIt MoveGroup Interface
  using moveit::planning_interface::MoveGroupInterface;
  auto move_group_interface = MoveGroupInterface(node, "ur_manipulator");
  moveit_msgs::msg::JointConstraint joint_constraint;
  moveit_msgs::msg::Constraints path_constraints;
  std::vector<std::string> joint_names = {
    "shoulder_pan_joint", "shoulder_lift_joint", "elbow_joint", 
    "wrist_1_joint", "wrist_2_joint"
  };

  double joint_values[5][3] = {
    {5.0/8.0 * M_PI, 5.0/8.0*M_PI, 5.0/8.0 * M_PI},
    {-M_PI_2, M_PI_2, M_PI_2},
    {M_PI_2, M_PI_2, 3.0/8.0*M_PI},
    {-M_PI_2, 9.0/16.0*M_PI, 9.0/16.0 * M_PI},
    {-M_PI_2, M_PI_2, M_PI_2},
  }; 
  // for (std::size_t i = 0; i < joint_names.size(); i++) {
  //   moveit_msgs::msg::JointConstraint joint_constraint;
  //   joint_constraint.joint_name = joint_names[i];
  //   joint_constraint.position = joint_values[i][0];
  //   joint_constraint.tolerance_above = joint_values[i][1];
  //   joint_constraint.tolerance_below = joint_values[i][2];
  //   joint_constraint.weight = 1.0;
  //   path_constraints.joint_constraints.push_back(joint_constraint);
  // };

    // joint_constraint.joint_name = "shoulder_pan_joint";
    // joint_constraint.position = (5.0/8.0) * M_PI;
    // joint_constraint.tolerance_above = (5.0/8.0) * M_PI;
    // joint_constraint.tolerance_below = (5.0/8.0) * M_PI;
    // joint_constraint.weight = 1.0;
    // path_constraints.joint_constraints.push_back(joint_constraint);

  // moveit_msgs::msg::OrientationConstraint o_constraint;
  // o_constraint.link_name = "tool0"; // Change this if needed
  // o_constraint.header.frame_id = "base_link"; 
  tf2::Quaternion quat;
  // quat.setRPY(0, M_PI, 0);  // Roll, Pitch, Yaw (RPY)
  // o_constraint.orientation.x = quat.x();
  // o_constraint.orientation.y = quat.y();
  // o_constraint.orientation.z = quat.z();
  // o_constraint.orientation.w = quat.w();
  // // Set strict orientation constraints (forces tool to stay aligned)
  // o_constraint.absolute_x_axis_tolerance = 0.1; // Small tolerance
  // o_constraint.absolute_y_axis_tolerance = 0.1;
  // o_constraint.absolute_z_axis_tolerance = M_PI; // Allow full wrist rotation
  // o_constraint.weight = 1.0;

  // Apply the orientation constraint
  // moveit_msgs::msg::Constraints constraints;
  // constraints.orientation_constraints.emplace_back(o_constraints);
  //move_group_interface.setPathConstraints(path_constraints);


  move_group_interface.setPlanningTime(5);
  move_group_interface.setNumPlanningAttempts(40);


  move_group_interface.setPoseReferenceFrame("chess_frame");
  

  // Set target position
  geometry_msgs::msg::Pose target_pose;
  target_pose.position.x = 0.00;     
  target_pose.position.y = 0.00;
  target_pose.position.z = 0.05;


  quat.setRPY(0, M_PI, 0);  // Roll, Pitch, Yaw (RPY)
  target_pose.orientation.x = quat.x();
  target_pose.orientation.y = quat.y();
  target_pose.orientation.z = quat.z();
  target_pose.orientation.w = quat.w();


  move_group_interface.setPoseTarget(target_pose);

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

  target_pose.position.x = 0.0;     
  target_pose.position.y = 0.30;
  target_pose.position.z = 0.35; 

  move_group_interface.setPoseTarget(target_pose);


  // Create a plan to that target pose
  auto const [success1, plan1] = [&move_group_interface]{
    moveit::planning_interface::MoveGroupInterface::Plan msg;
    auto const ok = static_cast<bool>(move_group_interface.plan(msg));
    return std::make_pair(ok, msg);
  }();

  // Execute the plan
  if(success1) {
    move_group_interface.execute(plan1);
  } else {
    RCLCPP_ERROR(logger, "Planing failed!");
  }


  target_pose.position.x = 0.15;     
  target_pose.position.y = 0.30;
  target_pose.position.z = 0.05; 


  move_group_interface.setPoseTarget(target_pose);


  // Create a plan to that target pose
  auto const [success2, plan2] = [&move_group_interface]{
    moveit::planning_interface::MoveGroupInterface::Plan msg;
    auto const ok = static_cast<bool>(move_group_interface.plan(msg));
    return std::make_pair(ok, msg);
  }();

  // Execute the plan
  if(success2) {
    move_group_interface.execute(plan2);
  } else {
    RCLCPP_ERROR(logger, "Planing failed!");
  }
  

  // Shutdown ROS
  rclcpp::shutdown();
  return 0;
}