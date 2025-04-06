from launch import LaunchDescription
from launch_ros.actions import Node
from moveit_configs_utils import MoveItConfigsBuilder

def generate_launch_description():
    # Set MoveIt configuration
    moveit_config = MoveItConfigsBuilder("ur").to_moveit_configs()

    # Define simple_move node
    simple_move_node = Node(
        package="ur5_sem2_scripts",  # Your package name
        executable="simple_move",  # Your script name
        output="screen",
        parameters=[
            moveit_config.robot_description,  # Load URDF
            moveit_config.robot_description_semantic,  # Load SRDF
            moveit_config.robot_description_kinematics,  # Load kinematics.yaml
        ],
    )

    return LaunchDescription([simple_move_node])
