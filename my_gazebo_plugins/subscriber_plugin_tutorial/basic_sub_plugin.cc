#include <gazebo/gazebo.hh>
#include <gazebo/common/common.hh>
#include <gazebo/physics/physics.hh>

namespace gazebo
{
    class myGazeboSubscriberPlugin : public ModelPlugin
    {
        public:
            myGazeboSubscriberPlugin() : ModelPlugin()
            {
                // std::cout << "My subscriber plugin created" << std::endl;
                printf("My subscriber plugin created\n");
            }

        public:
            void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf)
            {
                // print the name of the model tag
                std::cout << "name of the model tag: " << _model->GetName() << std::endl;

                // create a node
                transport::NodePtr myNode(new transport::Node());
                myNode->Init();


                // myNode subscribing to ~/world_stats topic
                // the subcriber pointer isn't even used
                // implicit template types - gets the type from the arguments
                // have to set the pointer to a member variable or else the object gets destroyed after 
                // the Load scope ends
                this->m_mySubPtr = myNode->Subscribe(
                    "~/world_stats",
                    &myGazeboSubscriberPlugin::On_msg,
                    this);
                // transport::SubscriberPtr mySubPtr = myNode->Subscribe(
                //     "~/world_stats", 
                //     &myGazeboSubscriberPlugin::On_msg, 
                //     this);
                // // or explicit template type
                // // transport::SubscriberPtr mySubPtr = myNode->Subscribe
                // //     <gazebo::msgs::WorldStatistics, gazebo::myGazeboSubscriberPlugin>(
                // //     "~/world_stats", 
                // //     &myGazeboSubscriberPlugin::On_msg, 
                // //     this);
            }
        
        public:
            void On_msg(ConstWorldStatisticsPtr& _msg)
            {
                // WorldStatistics inherits ::google::protobuf::Message
                // ::google::protobuf::Message is in the global namespace i.e. outside gazebo namespace
                // so _msg is able to use DebugString()
                std::cout << _msg->DebugString();
            }

        private:
            transport::SubscriberPtr m_mySubPtr;
    
    };
    GZ_REGISTER_MODEL_PLUGIN(myGazeboSubscriberPlugin)
}
