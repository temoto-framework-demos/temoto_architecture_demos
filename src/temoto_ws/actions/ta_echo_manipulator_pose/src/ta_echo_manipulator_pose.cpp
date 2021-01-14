
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *
 *  The basis of this file has been automatically generated
 *  by the TeMoto action package generator. Modify this file
 *  as you wish but please note:
 *
 *    WE HIGHLIY RECOMMEND TO REFER TO THE TeMoto ACTION
 *    IMPLEMENTATION TUTORIAL IF YOU ARE UNFAMILIAR WITH
 *    THE PROCESS OF CREATING CUSTOM TeMoto ACTION PACKAGES
 *    
 *  because there are plenty of components that should not be
 *  modified or which do not make sence at the first glance.
 *
 *  See TeMoto documentation & tutorials at: 
 *    https://temoto-telerobotics.github.io
 *
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

/* REQUIRED BY TEMOTO */
#include <class_loader/class_loader.hpp>
#include "ta_echo_manipulator_pose/temoto_action.h"
#include "ta_echo_manipulator_pose/macros.h"
#include "temoto_robot_manager/robot_manager_interface.h"

/* 
 * ACTION IMPLEMENTATION of TaEchoManipulatorPose 
 */
class TaEchoManipulatorPose : public TemotoAction
{
public:

// Constructor. REQUIRED BY TEMOTO
TaEchoManipulatorPose()
{
  std::cout << __func__ << " constructed\n";
}

// REQUIRED BY TEMOTO
void executeTemotoAction()
{
  // Input parameters
  std::string in_param_robot_name = GET_PARAMETER("robot_name", std::string);

  rmi_.initialize(this);
  ros::Duration(10).sleep();
  geometry_msgs::PoseStamped eef_pose;  
  eef_pose.pose = rmi_.getEndEffPose(in_param_robot_name);
  TEMOTO_INFO_STREAM("EEF pose of '" << in_param_robot_name << "':" << std::endl << eef_pose.pose);
}

// Destructor
~TaEchoManipulatorPose()
{
  TEMOTO_INFO("Action instance destructed");
}

robot_manager::RobotManagerInterface<TaEchoManipulatorPose> rmi_;

}; // TaEchoManipulatorPose class

/* REQUIRED BY CLASS LOADER */
CLASS_LOADER_REGISTER_CLASS(TaEchoManipulatorPose, ActionBase);
