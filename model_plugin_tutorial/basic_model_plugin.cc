#include <gazebo/gazebo.hh>
#include <gazebo/common/common.hh>
#include <gazebo/physics/physics.hh>
#include <ignition/math/Vector3.hh>

namespace gazebo
{
    class MyModelPlugin : public ModelPlugin
    {
        private:
            int count = 0;
            double vel = 0;
        
        private:
            physics::ModelPtr myModel;
            event::ConnectionPtr updateConnection;

        public:
            MyModelPlugin() : ModelPlugin()
            {
                std::cout << "My model plugin started" << std::endl;
            }

        public:
            void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf)
            {
                this->myModel = _model;

                std::cout << "My model name is: " << myModel->GetName() << std::endl;

                // check if the sdf has a tag tag called model_vel
                // HasElement() returns a bool
                if(_sdf->HasElement("model_vel"))
                {
                    // explicit template function
                    this->vel = _sdf->Get<double>("model_vel");

                    // implicit template function
                    // this->vel = _sdf->Get("model_vel");
                }

                // print the value of the velocity to the terminal
                std::cout << "Model velocity set to: " << this->vel << std::endl;

                // callback member function
                // the type is std::_Bind<void (gazebo::MyModelPlugin::*(gazebo::MyModelPlugin *))()>
                // use auto since the type is "unknowable" i.e. not specified in the doc
                // to bind member functions, need a pointer to member function and its class instance
                auto updateFunc = std::bind(&MyModelPlugin::onUpdate, this);

                // the below two do not work
                // void(MyModelPlugin::*updateFunc)() = &MyModelPlugin::onUpdate;
                // std::function<void()> updateFunc = std::bind(&MyModelPlugin::onUpdate, this);

                // on start of simulation start calling the onUpdate function
                this->updateConnection = event::Events::ConnectWorldUpdateBegin(updateFunc);
            }

        public:
            void onUpdate()
            {
                if(this->count < 10000)
                {
                    this->myModel->SetLinearVel(ignition::math::Vector3d(0,0,this->vel));
                }

                this->count++;
            }
    };
    GZ_REGISTER_MODEL_PLUGIN(MyModelPlugin)
}