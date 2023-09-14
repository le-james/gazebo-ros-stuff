#include <gazebo/gazebo.hh>
#include <gazebo/common/common.hh>
#include <gazebo/physics/physics.hh>
#include <ignition/math/Vector3.hh>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/bool.hpp"
#include "gazebo_ros/node.hpp"



namespace gazebo
{
    class MyROSModelPlugin : public ModelPlugin//, public rclcpp::Node
    {
        // for gazebo model
        private:
            int count = 0;
            double xVel = 0;
            double yVel = 0;
            double zVel = 0;
        
        // for gazebo sim
        private:
            physics::ModelPtr m_myModel;
            event::ConnectionPtr updateConnection;

        // for ros subscriber
        private:
            bool activate_move = false;

            // gazebo_ros a SharedPtr alias using the keyword "using"
            gazebo_ros::Node::SharedPtr ros_node;
            // or 
            // use the rclcpp SharedPtr created using a marco
            // rclcpp::Node::SharedPtr ros_node;

            // rclcpp creates a SharedPtr alias using a macro
            rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr m_sub;
            rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr m_sub2;
            // or 
            // use the full type
            // std::shared_ptr<rclcpp::Subscription<std_msgs::msg::Bool>> m_sub;

        // constructor
        public:
            MyROSModelPlugin() : ModelPlugin()
            {
                std::cout << "My model plugin started" << std::endl;
            }

        // member functions
        public:
            void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf)
            {
                // need to include this for the callback with arguments - has to be in this scope
                using std::placeholders::_1;

                // implementing ros in a gazebo plugin start
                // could also create the subscriber node in the constuctor too

                // create a gazebo plugin ros node
                // Get() is only in the namespace of gazebo_ros and not in the namespace rclcpp::Node
                this->ros_node = gazebo_ros::Node::Get(_sdf);

                // create a node to subscribe to the ros topic i.e. gazebo subscribes to it
                // std::function<void(const std_msgs::msg::Bool::ConstSharedPtr)> rosCallback = 
                //     std::bind(&MyROSModelPlugin::Activate_Callback,this,_1);

                this->m_sub = ros_node->create_subscription<std_msgs::msg::Bool>(
                    "/my_model_move_up",
                    10,
                    // std::bind(&MyROSModelPlugin::Activate_Callback,this,_1)
                    std::bind(&MyROSModelPlugin::Activate_Callback,this,_1)
                );
                // or
                // using lambda function
                this->m_sub2 = ros_node->create_subscription<std_msgs::msg::Bool>(
                    "/my_model_move_sideways",
                    10,
                    [&](const std_msgs::msg::Bool::SharedPtr msg) {
                        RCLCPP_INFO(this->ros_node->get_logger(), "Received message: %d", msg->data);
                        this->activate_move = msg->data;

                        this->xVel = 0.5;
                        this->yVel = 0.5;
                        this->zVel = 1;
                    }
                );
                
                RCLCPP_INFO(this->ros_node->get_logger(), "ROS model plugin loaded!");
                
                // implementing ros in a gazebo plugin end



                // from model plugin start
                this->m_myModel = _model;

                std::cout << "My model name is: " << m_myModel->GetName() << std::endl;

                // check if the sdf has a tag tag called model_vel
                // HasElement() returns a bool
                if(_sdf->HasElement("model_vel"))
                {
                    // get the velocity from the the <model_vel> tag

                    // explicit template function
                    this->xVel = _sdf->Get<double>("model_vel");
                    this->yVel = _sdf->Get<double>("model_vel");
                    this->zVel = _sdf->Get<double>("model_vel");

                    // implicit template function
                    // this->vel = _sdf->Get("model_vel");
                }



                // testing model and element pointers

                // both do the same thing here
                // get the link by name - GetName() just prints the name "box"
                gazebo::physics::LinkPtr link = _model->GetLink("box");
                std::cout << link->GetName() << std::endl;
                // convert ModelPtr to a ElementPtr - GetElement() only returns the first link tag it sees
                sdf::v9::ElementPtr linkAlt = this->m_myModel->GetSDF()->GetElement("link");
                std::cout << linkAlt->Get<std::string>("name") << std::endl;

                // ElementPtr only points to tags inside the plugin tag
                std::cout << _sdf->GetElement("test")->GetName() << std::endl;
                std::cout << _sdf->GetElement("test")->GetAttribute("name")->GetAsString() << std::endl;



                // print the value of the velocity to the terminal
                std::cout << "Model x velocity initially set to: " << this->xVel << std::endl;
                std::cout << "Model y velocity initially set to: " << this->yVel << std::endl;
                std::cout << "Model z velocity initially set to: " << this->zVel << std::endl;

                // callback member function
                // the type is std::_Bind<void (gazebo::MyROSModelPlugin::*(gazebo::MyROSModelPlugin *))()>
                // use auto since the type is "unknowable" i.e. not specified in the doc
                // to bind member functions, need a pointer to member function and its class instance
                auto updateFunc = std::bind(&MyROSModelPlugin::onUpdate, this);

                // the below two do not work
                // void(MyROSModelPlugin::*updateFunc)() = &MyROSModelPlugin::onUpdate;
                // std::function<void()> updateFunc = std::bind(&MyROSModelPlugin::onUpdate, this);

                // on start of simulation start calling the onUpdate function
                this->updateConnection = event::Events::ConnectWorldUpdateBegin(updateFunc);
                // from model plugin end
            }

        public:
            // void Activate_Callback(const std_msgs::msg::Bool::ConstPtr& msg)
            // {
            //     this->activate_move = msg->data;
            // }
            // cannot change the pointer and the value that the pointer points to
            void Activate_Callback(const std_msgs::msg::Bool::ConstSharedPtr msg)
            {
                RCLCPP_INFO(this->ros_node->get_logger(), "Received message: %d", msg->data);

                this->activate_move = msg->data;
                
                this->xVel = 0;
                this->yVel = 0;
                this->zVel = 0.1;
            }

        public:
            void onUpdate()
            {
                // if activate_move is true then move the model in gazebo
                if(this->activate_move)
                {
                    if(this->count < 10000)
                    {
                        // applies the velocity using the global coordinate
                        this->m_myModel->SetLinearVel(ignition::math::Vector3d(this->xVel,this->yVel,this->zVel));
                    }
                    else
                    {
                        // reset if past 10000
                        activate_move = false;
                        count = 0;
                    }

                    this->count++;
                }
            }
    };
    GZ_REGISTER_MODEL_PLUGIN(MyROSModelPlugin)
}
