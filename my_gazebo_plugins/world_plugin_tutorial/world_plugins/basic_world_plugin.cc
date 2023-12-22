#include <gazebo/gazebo.hh>
#include <gazebo/common/common.hh>
#include <gazebo/physics/physics.hh>
#include <ignition/math/Pose3.hh>

namespace gazebo
{
    class myGazeboWorldPlugin : public WorldPlugin
    {
        public:
            myGazeboWorldPlugin() : WorldPlugin()
            {
                printf("Hello my world plugin's constructor\n");
            }

        public:
            // implementing the pure virtual function Load from the WorldPlugin() class
            // _world points to the world tag in the sdf file and _sdf points to the sdf tag
            void Load(physics::WorldPtr _world, sdf::ElementPtr _sdf)
            {
                // get the name of the world tag in the sdf file
                std::cout << "The world name is: " << _world->Name() << std::endl;

                // creates a node
                // the Node class is inside the transport namespace
                transport::NodePtr myNode(new transport::Node());

                // myNode->Init(_world->Name());
                // the string must match the world tag's name in the world file
                // "myWRLD" is the name of the world tag in the cpp cart pole workspace
                myNode->Init("myWRLD");

                // this creates a topic
                    // advertise (from ros1) is basically equivalent to creating a publisher in ros2
                    // this topic is automatically created by gazebo even if we don't do this
                    // we do this to be able to get the pointer and publish on to it
                // calling a template function: nameOfFunc<type>(function's argument)
                // msgs is a namespace but it doesn't have the Factory class in the API for some reason
                // the gazebo API is in ~/gazebo-11/gazebo
                transport::PublisherPtr myPublisher = myNode->Advertise<msgs::Factory>("~/factory");
                // or
                // transport::PublisherPtr myPublisher = myNode->Advertise<gazebo::msgs::Factory>("~/factory");

                // instantiate a Factory class froms the msgs namespace
                msgs::Factory msg;

                // model to use
                msg.set_sdf_filename("model://simple_arm");

                // set the pose of the model
                msgs::Set(msg.mutable_pose(), ignition::math::Pose3d(1, 1, 0, 0, 0, 0));

                myPublisher->Publish(msg);

            }
    };
    // register the plugin
    GZ_REGISTER_WORLD_PLUGIN(myGazeboWorldPlugin)
}
