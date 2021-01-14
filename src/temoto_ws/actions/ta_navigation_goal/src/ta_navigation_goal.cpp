
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
#include "ta_navigation_goal/temoto_action.h"
#include "ta_navigation_goal/macros.h"
#include "temoto_robot_manager/robot_manager_interface.h"
#include "tf/tf.h"

/* 
 * ACTION IMPLEMENTATION of TaNavigationGoal 
 */
class TaNavigationGoal : public TemotoAction
{
public:

// Constructor. REQUIRED BY TEMOTO
TaNavigationGoal()
{
  std::cout << __func__ << " constructed\n";
}

// REQUIRED BY TEMOTO
void executeTemotoAction()
{
  // Input parameters
  std::string in_param_robot_name = GET_PARAMETER("robot_name", std::string);
  std::string in_param_nav_goal_frame_id = GET_PARAMETER("nav_goal::frame_id", std::string);
  double in_param_nav_goal_orientation_yaw = GET_PARAMETER("nav_goal::orientation::yaw", double);
  double in_param_nav_goal_position_x = GET_PARAMETER("nav_goal::position::x", double);
  double in_param_nav_goal_position_y = GET_PARAMETER("nav_goal::position::y", double);

  rmi_.initialize(this);

  geometry_msgs::PoseStamped target_pose;
  target_pose.pose.position.x = in_param_nav_goal_position_x;
  target_pose.pose.position.y = in_param_nav_goal_position_y;
  target_pose.pose.orientation = tf::createQuaternionMsgFromRollPitchYaw(0, 0, in_param_nav_goal_orientation_yaw);;

  TEMOTO_INFO_STREAM("Sending a navigation goal to " << in_param_robot_name << " ...");
  rmi_.navigationGoal(in_param_robot_name, in_param_nav_goal_frame_id, target_pose);
  TEMOTO_INFO_STREAM("Done navigating");
}

// Destructor
~TaNavigationGoal()
{
  TEMOTO_INFO("Action instance destructed");
}

robot_manager::RobotManagerInterface<TaNavigationGoal> rmi_;

}; // TaNavigationGoal class

/* REQUIRED BY CLASS LOADER */
CLASS_LOADER_REGISTER_CLASS(TaNavigationGoal, ActionBase);
