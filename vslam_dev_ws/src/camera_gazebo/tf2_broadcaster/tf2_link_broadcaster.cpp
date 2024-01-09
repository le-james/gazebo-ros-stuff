#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/transform_broadcaster.h"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "geometry_msgs/msg/pose.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "geometry_msgs/msg/pose_array.hpp"

// the gazebo plugin "gazebo_ros_link_pose_publisher" has this code in it
// it is launched in the launch file
// not sure why i did this

class FramePublisher : public rclcpp::Node
{
    private:
        rclcpp::Subscription<geometry_msgs::msg::PoseArray>::SharedPtr subscription_;
        // rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr subscription_;

        std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;

    public:
        FramePublisher() : Node("tf2_link_broadcaster")
        {

            // Initialize the transform broadcaster
            tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);

            subscription_ = this->create_subscription<geometry_msgs::msg::PoseArray>(
            // subscription_ = this->create_subscription<geometry_msgs::msg::PoseStamped>(
                "/link_pose", 10,
                std::bind(&FramePublisher::handle_link_pose, this, std::placeholders::_1));
        }

    private:
        void handle_link_pose(const std::shared_ptr<geometry_msgs::msg::PoseArray> msg)
        // void handle_link_pose(const std::shared_ptr<geometry_msgs::msg::PoseStamped> msg)
        {
            geometry_msgs::msg::TransformStamped t;
            // geometry_msgs::msg::TransformStamped t2;

            // Read message content and assign it to
            // corresponding tf variables

            // t.header.stamp = this->get_clock()->now();
            // t.header.stamp = rclcpp::Node::now();
            // rclcpp::Time time = rclcpp::Node::now();
            t.header.stamp = this->now();

            t.header.frame_id = "world";
            t.child_frame_id = "base_link";
            // t.child_frame_id = "slider_link";





            // t2.header.stamp = this->get_clock()->now();
            // t2.header.frame_id = "base_link";
            // t2.child_frame_id = "slider_link";


            // base link position of the link from gazebo
            t.transform.translation.x = msg->poses[0].position.x;
            t.transform.translation.y = msg->poses[0].position.y;
            t.transform.translation.z = msg->poses[0].position.z;

            // base link rotation of the link from gazebo
            t.transform.rotation.x = msg->poses[0].orientation.x;
            t.transform.rotation.y = msg->poses[0].orientation.y;
            t.transform.rotation.z = msg->poses[0].orientation.z;
            t.transform.rotation.w = msg->poses[0].orientation.w;



            // slider link position of the link from gazebo
            // t.transform.translation.x = msg->poses[1].position.x;
            // t.transform.translation.y = msg->poses[1].position.y;
            // t.transform.translation.z = msg->poses[1].position.z;

            // // slider link rotation of the link from gazebo
            // t.transform.rotation.x = msg->poses[1].orientation.x;
            // t.transform.rotation.y = msg->poses[1].orientation.y;
            // t.transform.rotation.z = msg->poses[1].orientation.z;
            // t.transform.rotation.w = msg->poses[1].orientation.w;


            // slider link position of the link from gazebo
            // t.transform.translation.x = msg->poses[0].position.x;
            // t.transform.translation.y = msg->poses[0].position.y;
            // t.transform.translation.z = msg->poses[0].position.z;

            // // slider link rotation of the link from gazebo
            // t.transform.rotation.x = msg->poses[0].orientation.x;
            // t.transform.rotation.y = msg->poses[0].orientation.y;
            // t.transform.rotation.z = msg->poses[0].orientation.z;
            // t.transform.rotation.w = msg->poses[0].orientation.w;


            // // slider link position of the link from gazebo
            // t2.transform.translation.x = msg->poses[0].position.x;
            // t2.transform.translation.y = msg->poses[0].position.y;
            // t2.transform.translation.z = msg->poses[0].position.z;

            // // slider link rotation of the link from gazebo
            // t2.transform.rotation.x = msg->poses[0].orientation.x;
            // t2.transform.rotation.y = msg->poses[0].orientation.y;
            // t2.transform.rotation.z = msg->poses[0].orientation.z;
            // t2.transform.rotation.w = msg->poses[0].orientation.w;


            // from pose not pose array
            // position of the link from gazebo
            // t.transform.translation.x = msg->pose.position.x;
            // t.transform.translation.y = msg->pose.position.y;
            // t.transform.translation.z = msg->pose.position.z;

            // // rotation of the link from gazebo
            // t.transform.rotation.x = msg->pose.orientation.x;
            // t.transform.rotation.y = msg->pose.orientation.y;
            // t.transform.rotation.z = msg->pose.orientation.z;
            // t.transform.rotation.w = msg->pose.orientation.w;

            // Send the transformation
            tf_broadcaster_->sendTransform(t);
        }
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<FramePublisher>());
    rclcpp::shutdown();
    return 0;
}