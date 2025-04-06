#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <tf2_ros/static_transform_broadcaster.h>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <tf2/LinearMath/Quaternion.h>

class StaticTransformPublisher : public rclcpp::Node
{
public:
    StaticTransformPublisher()
        : Node("static_transform_publisher")
    {
        // Create a static transform broadcaster
        static_broadcaster_ = std::make_shared<tf2_ros::StaticTransformBroadcaster>(this);

        // Create the Transform message
        geometry_msgs::msg::TransformStamped transformStamped;

        // Set frame names
        transformStamped.header.stamp = this->now();
        transformStamped.header.frame_id = "svejse_frame";
        transformStamped.child_frame_id = "chess_frame";

        // Set translation (no translation)
        transformStamped.transform.translation.x = 0.30;
        transformStamped.transform.translation.y = -0.20;
        transformStamped.transform.translation.z = 0.0;

        // Set rotation (112.5 degrees = 1.9635 radians around the Z axis)
        tf2::Quaternion quat;
        quat.setRPY(0, 0, 0);  // Roll, Pitch, Yaw (RPY)
        transformStamped.transform.rotation.x = quat.x();
        transformStamped.transform.rotation.y = quat.y();
        transformStamped.transform.rotation.z = quat.z();
        transformStamped.transform.rotation.w = quat.w();

        // Publish the static transform
        static_broadcaster_->sendTransform(transformStamped);

        RCLCPP_INFO(this->get_logger(), "Static Transform Published: base_link -> my_frame with 112.5-degree rotation around Z axis");
    }

private:
    std::shared_ptr<tf2_ros::StaticTransformBroadcaster> static_broadcaster_;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<StaticTransformPublisher>());
    rclcpp::shutdown();
    return 0;
}
