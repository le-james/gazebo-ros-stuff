#include <gazebo/gazebo.hh>
#include <gazebo/common/common.hh>
#include <gazebo/physics/physics.hh>
#include <ignition/math/Vector3.hh>

namespace gazebo
{
    class PropellerThrustModelPlugin : public ModelPlugin
    {
        private:
            int count = 0;
            double rotor_0_force = 0;
            double rotor_1_force = 0;
            double rotor_2_force = 0;
            double rotor_3_force = 0;
            double rotor_puller_force = 0;
        
        private:
            physics::ModelPtr myVTOL;
            event::ConnectionPtr updateConnection;
            event::ConnectionPtr updateConnection2;

        public:
            PropellerThrustModelPlugin() : ModelPlugin()
            {
                std::cout << "Propeller plugin started!" << std::endl;
            }

        public:
            void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf)
            {
                this->myVTOL = _model;

                std::cout << "My model name is: " << myVTOL->GetName() << std::endl;

                if(_sdf->HasElement("applied_force"))
                {
                    this->rotor_0_force = _sdf->Get<double>("rotor_0_force");
                    this->rotor_1_force = _sdf->Get<double>("rotor_1_force");
                    this->rotor_2_force = _sdf->Get<double>("rotor_2_force");
                    this->rotor_3_force = _sdf->Get<double>("rotor_3_force");
                    this->rotor_puller_force = _sdf->Get<double>("rotor_puller_force");
                }

                // std::cout << "Every propeller applies a force of: " << this->force << std::endl;

                // callback member function
                auto updateFunc = std::bind(&PropellerThrustModelPlugin::onUpdate, this);
                auto updateFunc2 = std::bind(&PropellerThrustModelPlugin::onUpdate2, this);

                // on start of simulation start calling the onUpdate function
                this->updateConnection = event::Events::ConnectWorldUpdateBegin(updateFunc);
                this->updateConnection2 = event::Events::ConnectWorldUpdateBegin(updateFunc2);
            }

        public:
            void onUpdate()
            {
                if(this->count == 0)
                {
                //     // set forces once
                    this->myVTOL->GetLink("rotor_0")->AddRelativeForce(ignition::math::Vector3d(0,0,this->rotor_0_force));
                    // std::cout << "Force being applied to rotor_0: " << this->myVTOL->GetLink("rotor_0")->RelativeForce() << std::endl;
                    
                    this->myVTOL->GetLink("rotor_1")->AddRelativeForce(ignition::math::Vector3d(0,0,this->rotor_1_force));
                    this->myVTOL->GetLink("rotor_2")->AddRelativeForce(ignition::math::Vector3d(0,0,this->rotor_2_force));
                    this->myVTOL->GetLink("rotor_3")->AddRelativeForce(ignition::math::Vector3d(0,0,this->rotor_3_force));

                    this->myVTOL->GetLink("rotor_puller")->AddRelativeForce(ignition::math::Vector3d(0,0,this->rotor_puller_force));

                    // set forces once
                    // this->myVTOL->GetLink("rotor_0")->SetForce(ignition::math::Vector3d(0,0,this->rotor_0_force));
                    // this->myVTOL->GetLink("rotor_1")->SetForce(ignition::math::Vector3d(0,0,this->rotor_1_force));
                    // this->myVTOL->GetLink("rotor_2")->SetForce(ignition::math::Vector3d(0,0,this->rotor_2_force));
                    // this->myVTOL->GetLink("rotor_3")->SetForce(ignition::math::Vector3d(0,0,this->rotor_3_force));

                    // this->myVTOL->GetLink("rotor_puller")->SetForce(ignition::math::Vector3d(0,0,this->rotor_puller_force));

                }
            }

            void onUpdate2()
            {
                if(this->count == 0)
                {
                    // set forces once
                    // this->myVTOL->GetLink("rotor_puller")->AddRelativeForce(ignition::math::Vector3d(0,0,this->rotor_puller_force));
                }
            }

            // void onUpdate()
            // {
            //     if(this->count < 20000)
            //     {
            //         // force accumulates
            //         // this->myVTOL->GetLink("rotor_0")->AddRelativeForce(ignition::math::Vector3d(0,0,this->force));
            //         // this->myVTOL->GetLink("rotor_1")->AddRelativeForce(ignition::math::Vector3d(0,0,this->force));
            //         // this->myVTOL->GetLink("rotor_2")->AddRelativeForce(ignition::math::Vector3d(0,0,this->force));
            //         // this->myVTOL->GetLink("rotor_3")->AddRelativeForce(ignition::math::Vector3d(0,0,this->force));

            //         // set forces once
            //         this->myVTOL->GetLink("rotor_0")->SetForce(ignition::math::Vector3d(0,0,this->rotor_0_force));
            //         this->myVTOL->GetLink("rotor_1")->SetForce(ignition::math::Vector3d(0,0,this->rotor_1_force));
            //         this->myVTOL->GetLink("rotor_2")->SetForce(ignition::math::Vector3d(0,0,this->rotor_2_force));
            //         this->myVTOL->GetLink("rotor_3")->SetForce(ignition::math::Vector3d(0,0,this->rotor_3_force));

            //         std::cout << "Force being applied to rotor_0: " << this->myVTOL->GetLink("rotor_0")->RelativeForce() << std::endl;
            //         // std::cout << "Force being applied to rotor_0: " << this->myVTOL->GetLink("rotor_0")->WorldForce() << std::endl;
            //     }

            //     if(10000 < this->count < 20000)
            //     {
            //         this->myVTOL->GetLink("rotor_puller")->SetForce(ignition::math::Vector3d(0,0,this->rotor_puller_force));
            //     }

            //     this->count++;
            // }

            // void onUpdate()
            // {
            //     auto start_time = std::chrono::system_clock::now();

            //     while(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - start_time).count() < 1)
            //     {
            //         std::cout << "Force being applied to rotor_0: " << this->myVTOL->GetLink("rotor_0")->RelativeForce() << std::endl;
            //     }
            // }
    };
    GZ_REGISTER_MODEL_PLUGIN(PropellerThrustModelPlugin)
}