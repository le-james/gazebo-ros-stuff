#include <functional>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <ignition/math/Vector3.hh>

namespace gazebo
{
  class LinkForce : public ModelPlugin
  {
  
  
  
    public: LinkForce() : ModelPlugin()
    {
      printf("working?\n");
    }
 
  
  
    public: void Load(physics::ModelPtr _parent, sdf::ElementPtr /*_sdf*/)
    {
      // Store the pointer to the model
      this->model = _parent;

      // Listen to the update event. This event is broadcast every
      // simulation iteration.
      this->updateConnection = event::Events::ConnectWorldUpdateBegin(
          std::bind(&LinkForce::OnUpdate, this));
          

    }



    // Called by the world update start event
    public: void OnUpdate()
    {
      // Apply a small linear force to link thruster_1 and thruster_5
      this->model->GetLink("thruster_1")->AddRelativeForce(ignition::math::Vector3d(0,0,700));
      this->model->GetLink("thruster_5")->AddRelativeForce(ignition::math::Vector3d(0,0,300));
    }



    // Pointer to the model
    private: physics::ModelPtr model;



    // Pointer to the update event connection
    private: event::ConnectionPtr updateConnection;
    
    
    
  };

  // Register this plugin with the simulator
  GZ_REGISTER_MODEL_PLUGIN(LinkForce)
}
