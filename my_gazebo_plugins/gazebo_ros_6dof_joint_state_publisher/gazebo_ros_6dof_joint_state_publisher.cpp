#include <gazebo/gazebo.hh>
#include <gazebo/common/common.hh>
#include <gazebo/physics/physics.hh>
#include <ignition/math/Vector3.hh>

#include "rclcpp/rclcpp.hpp"
#include "gazebo_ros/node.hpp"

#include "sensor_msgs/msg/joint_state.hpp"
#include "geometry_msgs/msg/pose.hpp"


September 6
    Not need anymore because joints states is not actually needed
    I only needed the tf2 to get the pose of the links


namespace gazebo
{
    class Gazebo_Ros_6DOF_Joint_State_Publisher : public ModelPlugin
    {
        // for gazebo sim
        private:
            physics::ModelPtr m_myModel;
            event::ConnectionPtr updateConnection;

        // for gazebo ros publisher
        private:
            gazebo_ros::Node::SharedPtr ros_node;
            sensor_msgs::msg::JointState jointState;
            rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr m_pub;
            rclcpp::Subscription<geometry_msgs::msg::Pose>::SharedPtr m_sub;



        // constructor
        public:
            Gazebo_Ros_6DOF_Joint_State_Publisher() : ModelPlugin()
            {
                std::cout << "My joint state pub model plugin started" << std::endl;
            }

        // member functions
        public:
            void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf)
            {
                this->m_myModel = _model;

                std::cout << "My model name is: " << m_myModel->GetName() << std::endl;


                // physics::JointPtr jointPtr = m_myModel->GetJoint("slider_joint_x");
                // std::cout << "Joint Name: " << jointPtr->GetName() << std::endl;
                
                const physics::Joint_V jointVec = m_myModel->GetJoints();
                // index 0 is base_joint, index 1 is slider_joint_x
                std::cout << "Joint Name from vec: " << jointVec[1]->GetName() << std::endl;



                // implementing ros in a gazebo plugin start
                // could also create the subscriber node in the constuctor too

                // create a gazebo plugin ros node
                // Get() is only in the namespace of gazebo_ros and not in the namespace rclcpp::Node
                this->ros_node = gazebo_ros::Node::Get(_sdf);

                // create a node to subscribe to the ros topic i.e. gazebo subscribes to it
                // std::function<void(const std_msgs::msg::Bool::ConstSharedPtr)> rosCallback = 
                //     std::bind(&MyROSModelPlugin::Activate_Callback,this,_1);
                // using lambda function
                this->m_pub = ros_node->create_publisher<sensor_msgs::msg::JointState>("/joint_states",10);


                // sub using lambda function
                this->m_sub = ros_node->create_subscription<geometry_msgs::msg::Pose>(
                    "/link_pose",
                    10,
                    [&](const geometry_msgs::msg::Pose::SharedPtr pose_msg) {
                        // RCLCPP_INFO(this->ros_node->get_logger(), "Received message: %d", pose_msg->data);

                        // this->activate_move = msg->data;

                        // not sure if this works or not
                        // rclcpp::Time currTime = rclcpp::Time(0,0);
                        // jointState.header.stamp = currTime;
                        // std::cout << "time from crlcpp: " << currTime.seconds();



                        // jointState.name[0] = "base_joint";
                        // jointState.position[0] = 0;
                        // jointState.velocity[0] = 0;
                        // jointState.effort[0] = 0;

                        // jointState.name[1] = "slider_joint_x";
                        // jointState.position[1] = 1;
                        // jointState.velocity[1] = 0.05;
                        // jointState.effort[1] = 0;


                        // rclcpp::Time ros_time = ros_node->now();
                        // jointState.header.stamp = ros_time;

                        // jointState.name[0] = "slider_joint_x";
                        // jointState.position[0] = 1;
                        // jointState.velocity[0] = 0.05;
                        // jointState.effort[0] = 0;

                        // // publish the message
                        // this->m_pub->publish(jointState);
                    }
                );
            }
    };
    GZ_REGISTER_MODEL_PLUGIN(Gazebo_Ros_6DOF_Joint_State_Publisher)
}