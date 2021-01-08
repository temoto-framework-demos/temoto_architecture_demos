
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
#include "ta_xarm7_test/temoto_action.h"
#include "ta_xarm7_test/macros.h"
#include "temoto_robot_manager/robot_manager_interface.h"

/* 
 * ACTION IMPLEMENTATION of TaXarm7Test 
 */
class TaXarm7Test : public TemotoAction
{
public:

// Constructor. REQUIRED BY TEMOTO
TaXarm7Test()
{
  std::cout << __func__ << " constructed\n";
}

// REQUIRED BY TEMOTO
void executeTemotoAction()
{
  // Input parameters
  double in_param_pose_orientation_pitch = GET_PARAMETER("pose::orientation::pitch", double);
  double in_param_pose_orientation_roll = GET_PARAMETER("pose::orientation::roll", double);
  double in_param_pose_orientation_yaw = GET_PARAMETER("pose::orientation::yaw", double);
  double in_param_pose_position_x = GET_PARAMETER("pose::position::x", double);
  double in_param_pose_position_y = GET_PARAMETER("pose::position::y", double);
  double in_param_pose_position_z = GET_PARAMETER("pose::position::z", double);

  rmi_.initialize(this);
  std::string robot_name = "xarm7_robot";

  // xarm7_robot
  TEMOTO_INFO_STREAM("loading " << robot_name);
  rmi_.loadRobot(robot_name);
  TEMOTO_INFO_STREAM(robot_name << " initialized");

  rmi_.controlGripperPosition(robot_name,10);   //open
  ros::Duration(3).sleep();
  rmi_.controlGripperPosition(robot_name,100);   //partially close

  geometry_msgs::PoseStamped target_pose1;  
  target_pose1.pose = rmi_.getEndEffPose(robot_name);

  target_pose1.pose.position.x = 0.61288;
  target_pose1.pose.position.y = 0.042255;
  target_pose1.pose.position.z = 0.51192;
  rmi_.planManipulation(robot_name,"xarm7",target_pose1);  
  rmi_.executePlan(robot_name);

}

// Destructor
~TaXarm7Test()
{
  TEMOTO_INFO("Action instance destructed");
}

robot_manager::RobotManagerInterface<TaXarm7Test> rmi_;

}; // TaXarm7Test class

/* REQUIRED BY CLASS LOADER */
CLASS_LOADER_REGISTER_CLASS(TaXarm7Test, ActionBase);
