#include <gazebo/gazebo.hh>
#include <gazebo/common/common.hh>
#include <gazebo/physics/physics.hh>
#include <ignition/math/Vector3.hh>

#include "rclcpp/rclcpp.hpp"
#include "gazebo_ros/node.hpp"

#include "geometry_msgs/msg/pose.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "geometry_msgs/msg/pose_array.hpp"

#include "tf2_ros/transform_broadcaster.h"


namespace gazebo
{
    class gazebo_ros_link_pose_publisher : public ModelPlugin
    {
        // for gazebo sim
        private:
            physics::ModelPtr m_myModel;
            event::ConnectionPtr updateConnection;

        // for gazebo ros publisher
        private:
            gazebo_ros::Node::SharedPtr ros_node;
            rclcpp::Publisher<geometry_msgs::msg::PoseArray>::SharedPtr m_pub;
            // rclcpp::Publisher<geometry_msgs::msg::PoseStamped>::SharedPtr m_pub;


            rclcpp::Subscription<geometry_msgs::msg::PoseArray>::SharedPtr subscription_;
            std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;



        // constructor
        public:
            gazebo_ros_link_pose_publisher() : ModelPlugin()
            {
                std::cout << "Link Pose Publisher Starting..." << std::endl;
            }

        // member functions
        public:
            void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf)
            {
                this->m_myModel = _model;

                std::cout << "Starting link pose publisher" << std::endl;

                // need to include this for the callback with arguments - has to be in this scope
                using std::placeholders::_1;

                // implementing ros in a gazebo plugin start
                // could also create the subscriber node in the constuctor too

                // create a gazebo plugin ros node
                // Get() is only in the namespace of gazebo_ros and not in the namespace rclcpp::Node
                this->ros_node = gazebo_ros::Node::Get(_sdf);

                // create a publisher node
                this->m_pub = ros_node->create_publisher<geometry_msgs::msg::PoseArray>("/link_pose",10);
                // this->m_pub = ros_node->create_publisher<geometry_msgs::msg::PoseStamped>("/link_pose",10);

               

                // Initialize the transform broadcaster
                // tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);
                tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(ros_node);



                

                // callback member function
                // the type is std::_Bind<void (gazebo::MyROSModelPlugin::*(gazebo::MyROSModelPlugin *))()>
                // use auto since the type is "unknowable" i.e. not specified in the doc
                // to bind member functions, need a pointer to member function and its class instance
                auto updateFunc = std::bind(&gazebo_ros_link_pose_publisher::onUpdate, this);

                // the below two do not work
                // void(MyROSModelPlugin::*updateFunc)() = &MyROSModelPlugin::onUpdate;
                // std::function<void()> updateFunc = std::bind(&MyROSModelPlugin::onUpdate, this);

                // on start of simulation start calling the onUpdate function
                this->updateConnection = event::Events::ConnectWorldUpdateBegin(updateFunc);
            }


        public:
            void onUpdate()
            {
                // get base link pose and rot
                // ignition::math::Vector3 base_link_pos = this->m_myModel->GetLink("base_link")->WorldPose().Pos();
                // ignition::math::Quaternion base_link_rot = this->m_myModel->GetLink("base_link")->WorldPose().Rot();
                ignition::math::Vector3 link_pos = this->m_myModel->GetLink("base_link")->WorldPose().Pos();
                ignition::math::Quaternion link_rot = this->m_myModel->GetLink("base_link")->WorldPose().Rot();

                // get slider link pose and rot
                // ignition::math::Vector3 slider_link_pos = this->m_myModel->GetLink("slider_link")->WorldPose().Pos();
                // ignition::math::Quaternion slider_link_rot = this->m_myModel->GetLink("slider_link")->WorldPose().Rot();


                // using regular pose - does this output the parent pose?
                // ignition::math::Vector3 link_pos = this->m_myModel->WorldPose().Pos();
                // ignition::math::Quaternion link_rot = this->m_myModel->WorldPose().Rot();


                // create the pose struct
                geometry_msgs::msg::PoseArray pose_vec = geometry_msgs::msg::PoseArray();
                // geometry_msgs::msg::PoseStamped link_pose = geometry_msgs::msg::PoseStamped();


                rclcpp::Time time = ros_node->now();

                // link_pose.header.stamp = time;
                // link_pose.header.frame_id = this->m_myModel->GetName();
                pose_vec.header.stamp = time;
                pose_vec.header.frame_id = this->m_myModel->GetName();


                pose_vec.poses.resize(1);

                // base link pose
                // pose_vec.poses[0].position.x = base_link_pos.X();
                // pose_vec.poses[0].position.y = base_link_pos.Y();
                // pose_vec.poses[0].position.z = base_link_pos.Z();

                // pose_vec.poses[0].orientation.x = base_link_rot.X();
                // pose_vec.poses[0].orientation.y = base_link_rot.Y();
                // pose_vec.poses[0].orientation.z = base_link_rot.Z();
                // pose_vec.poses[0].orientation.w = base_link_rot.W();

                // slider link pose
                // pose_vec.poses[0].position.x = slider_link_pos.X();
                // pose_vec.poses[0].position.y = slider_link_pos.Y();
                // pose_vec.poses[0].position.z = slider_link_pos.Z();

                // pose_vec.poses[0].orientation.x = slider_link_rot.X();
                // pose_vec.poses[0].orientation.y = slider_link_rot.Y();
                // pose_vec.poses[0].orientation.z = slider_link_rot.Z();
                // pose_vec.poses[0].orientation.w = slider_link_rot.W();
                
                // slider link pose
                // pose_vec.poses[1].position.x = slider_link_pos.X();
                // pose_vec.poses[1].position.y = slider_link_pos.Y();
                // pose_vec.poses[1].position.z = slider_link_pos.Z();

                // pose_vec.poses[1].orientation.x = slider_link_rot.X();
                // pose_vec.poses[1].orientation.y = slider_link_rot.Y();
                // pose_vec.poses[1].orientation.z = slider_link_rot.Z();
                // pose_vec.poses[1].orientation.w = slider_link_rot.W();

                // model parent link pose
                pose_vec.poses[0].position.x = link_pos.X();
                pose_vec.poses[0].position.y = link_pos.Y();
                pose_vec.poses[0].position.z = link_pos.Z();

                pose_vec.poses[0].orientation.x = link_rot.X();
                pose_vec.poses[0].orientation.y = link_rot.Y();
                pose_vec.poses[0].orientation.z = link_rot.Z();
                pose_vec.poses[0].orientation.w = link_rot.W();

                this->m_pub->publish(pose_vec);


                // fill in the struct with pos and rot data
                // link_pose.pose.position.x = link_pos.X();
                // link_pose.pose.position.y = link_pos.Y();
                // link_pose.pose.position.z = link_pos.Z();

                // link_pose.pose.orientation.x = link_rot.X();
                // link_pose.pose.orientation.y = link_rot.Y();
                // link_pose.pose.orientation.z = link_rot.Z();
                // link_pose.pose.orientation.w = link_rot.W();

                // this->m_pub->publish(link_pose);



                geometry_msgs::msg::TransformStamped t;
    
                t.header.stamp = ros_node->now();

                t.header.frame_id = "world";
                t.child_frame_id = "base_link";

                // base link position of the link from gazebo
                t.transform.translation.x = link_pos.X();
                t.transform.translation.y = link_pos.Y();
                t.transform.translation.z = link_pos.Z();

                // base link rotation of the link from gazebo
                t.transform.rotation.x = link_rot.X();
                t.transform.rotation.y = link_rot.Y();
                t.transform.rotation.z = link_rot.Z();
                t.transform.rotation.w = link_rot.W();

                tf_broadcaster_->sendTransform(t);
            }

    
    };
    GZ_REGISTER_MODEL_PLUGIN(gazebo_ros_link_pose_publisher)
}