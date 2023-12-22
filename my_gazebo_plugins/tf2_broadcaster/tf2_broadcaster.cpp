#include <gazebo/gazebo.hh>
#include <gazebo/common/common.hh>
#include <gazebo/physics/physics.hh>
#include <ignition/math/Vector3.hh>
#include <ignition/math/Quaternion.hh>

// #include <functional>
// #include <memory>
// #include <sstream>
// #include <string>

#include "geometry_msgs/msg/transform_stamped.hpp"
#include "rclcpp/rclcpp.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_ros/transform_broadcaster.h"

#include "gazebo_ros/node.hpp"



SEPTEMBER 6 2023:
    THIS CODE DOES NOT WORK
    NOT WORKING ON IT ANYMORE
    CAN NOT HAVE A TF2 BROADCASTER IN A GAZEBO PLUGIN



namespace gazebo
{
    class TF2_Broadcaster : public ModelPlugin
    {
        private:

            physics::ModelPtr m_model;

            
            std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;


            // broadcaster node
            gazebo_ros::Node::SharedPtr bc_node;


            event::ConnectionPtr updateConnection;

        public:
            TF2_Broadcaster() : ModelPlugin()
            {
                std::cout << "Broadcaster started!" << std::endl;
            }

        public:
            void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf)
            {

                this->m_model = _model;


                // this->bc_node = gazebo_ros::Node::Get(_sdf);


                // Initialize the transform broadcaster
                tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);

                // callback member function
                // the type is std::_Bind<void (gazebo::MyROSModelPlugin::*(gazebo::MyROSModelPlugin *))()>
                // use auto since the type is "unknowable" i.e. not specified in the doc
                // to bind member functions, need a pointer to member function and its class instance
                auto updateFunc = std::bind(&TF2_Broadcaster::onUpdate, this);

                // the below two do not work
                // void(MyROSModelPlugin::*updateFunc)() = &MyROSModelPlugin::onUpdate;
                // std::function<void()> updateFunc = std::bind(&MyROSModelPlugin::onUpdate, this);

                // on start of simulation start calling the onUpdate function
                this->updateConnection = event::Events::ConnectWorldUpdateBegin(updateFunc);
            }
            
        private:
            void onUpdate()
            {
                geometry_msgs::msg::TransformStamped t;

                // Read message content and assign it to
                // corresponding tf variables
                rclcpp::Time ros_time = bc_node->now();
                t.header.stamp = ros_time;
                t.header.frame_id = "base_link";
                t.child_frame_id = "slider_link";

                // get the link pos and publish to the transfrom topic
                ignition::math::Vector3 pos = this->m_model->GetLink("slider_link")->WorldPose().Pos();
                t.transform.translation.x = pos[0];
                t.transform.translation.y = pos[1];
                t.transform.translation.z = pos[2];

                // get the link rot and publish to the transfrom topic
                ignition::math::Quaternion rot = this->m_model->GetLink("slider_link")->WorldPose().Rot();
                t.transform.rotation.x = rot.X();
                t.transform.rotation.y = rot.Y();
                t.transform.rotation.z = rot.Z();
                t.transform.rotation.w = rot.W();

                // Send the transformation
                tf_broadcaster_->sendTransform(t);
            }

    };
}

