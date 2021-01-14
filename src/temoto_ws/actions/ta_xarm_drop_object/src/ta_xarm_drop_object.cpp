
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
#include "ta_xarm_drop_object/temoto_action.h"
#include "ta_xarm_drop_object/macros.h"
#include "temoto_robot_manager/robot_manager_interface.h"

/* 
 * ACTION IMPLEMENTATION of TaXarmDropObject 
 */
class TaXarmDropObject : public TemotoAction
{
public:

// Constructor. REQUIRED BY TEMOTO
TaXarmDropObject()
{
  std::cout << __func__ << " constructed\n";
}

// REQUIRED BY TEMOTO
void executeTemotoAction()
{
  // Input parameters
  double in_param_gripping_width = GET_PARAMETER("gripping_width", double);
  double in_param_pose_orientation_w = GET_PARAMETER("pose::orientation::w", double);
  double in_param_pose_orientation_x = GET_PARAMETER("pose::orientation::x", double);
  double in_param_pose_orientation_y = GET_PARAMETER("pose::orientation::y", double);
  double in_param_pose_orientation_z = GET_PARAMETER("pose::orientation::z", double);
  double in_param_pose_position_x = GET_PARAMETER("pose::position::x", double);
  double in_param_pose_position_y = GET_PARAMETER("pose::position::y", double);
  double in_param_pose_position_z = GET_PARAMETER("pose::position::z", double);

  rmi_.initialize(this);
  std::string robot_name = "xarm7_robot";

  // xarm7_robot
  TEMOTO_INFO_STREAM("loading " << robot_name);
  rmi_.loadRobot(robot_name);
  TEMOTO_INFO_STREAM(robot_name << " initialized");

  geometry_msgs::PoseStamped target_pose;

  /*
   * Open the gripper
   */
  rmi_.controlGripperPosition(robot_name, 100);
  ros::Duration(1).sleep();

  /*
   * Move the arm above the grabbing pose
   */
  target_pose.pose.position.x = in_param_pose_position_x;
  target_pose.pose.position.y = in_param_pose_position_y;
  target_pose.pose.position.z = in_param_pose_position_z + 0.1;

  target_pose.pose.orientation.x = in_param_pose_orientation_x;
  target_pose.pose.orientation.y = in_param_pose_orientation_y;
  target_pose.pose.orientation.z = in_param_pose_orientation_z;
  target_pose.pose.orientation.w = in_param_pose_orientation_w;

  rmi_.planManipulation(robot_name, "xarm7", target_pose);  
  rmi_.executePlan(robot_name);
  ros::Duration(1).sleep();

  /*
   * Move the arm to the grabbing pose
   */
  target_pose.pose.position.x = in_param_pose_position_x;
  target_pose.pose.position.y = in_param_pose_position_y;
  target_pose.pose.position.z = in_param_pose_position_z;

  target_pose.pose.orientation.x = in_param_pose_orientation_x;
  target_pose.pose.orientation.y = in_param_pose_orientation_y;
  target_pose.pose.orientation.z = in_param_pose_orientation_z;
  target_pose.pose.orientation.w = in_param_pose_orientation_w;

  rmi_.planManipulation(robot_name, "xarm7", target_pose);  
  rmi_.executePlan(robot_name);
  ros::Duration(1).sleep();

  // Grab the object
  rmi_.controlGripperPosition(robot_name, in_param_gripping_width);
  ros::Duration(1).sleep();

  /*
   * Move the arm to the object dropping pose
   */
  target_pose.pose.position.x = 0.670029;
  target_pose.pose.position.y = -0.00239678;
  target_pose.pose.position.z = 0.384358;

  target_pose.pose.orientation.x = 0.999478;
  target_pose.pose.orientation.y = -0.00292073;
  target_pose.pose.orientation.z = 0.0295777;
  target_pose.pose.orientation.w = 0.0126963;

  rmi_.planManipulation(robot_name, "xarm7", target_pose);  
  rmi_.executePlan(robot_name);
  ros::Duration(1).sleep();

  // Realease the object
  rmi_.controlGripperPosition(robot_name, 100);
  ros::Duration(1).sleep();

  /*
   * Move the arm to the home pose
   */
  target_pose.pose.position.x = 0.27339;
  target_pose.pose.position.y = -0.0100824;
  target_pose.pose.position.z = 0.579062;

  target_pose.pose.orientation.x = -0.999896;
  target_pose.pose.orientation.y = -0.00694897;
  target_pose.pose.orientation.z = -0.0117531;
  target_pose.pose.orientation.w = 0.00464684;

  rmi_.planManipulation(robot_name, "xarm7", target_pose);  
  rmi_.executePlan(robot_name);
  ros::Duration(1).sleep();

}

// Destructor
~TaXarmDropObject()
{
  TEMOTO_INFO("Action instance destructed");
}

robot_manager::RobotManagerInterface<TaXarmDropObject> rmi_;

}; // TaXarmDropObject class

/* REQUIRED BY CLASS LOADER */
CLASS_LOADER_REGISTER_CLASS(TaXarmDropObject, ActionBase);
