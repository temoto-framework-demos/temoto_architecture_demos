
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
#include "ta_xarm7_manipulate_object/temoto_action.h"
#include "ta_xarm7_manipulate_object/macros.h"
#include "temoto_robot_manager/robot_manager_interface.h"

/* 
 * ACTION IMPLEMENTATION of TaXarm7ManipulateObject 
 */
class TaXarm7ManipulateObject : public TemotoAction
{
public:

// Constructor. REQUIRED BY TEMOTO
TaXarm7ManipulateObject()
{
  std::cout << __func__ << " constructed\n";
}

// REQUIRED BY TEMOTO
void executeTemotoAction()
{
  rmi_.initialize(this);
  std::string robot_name = "xarm7_robot";

  // xarm7_robot
  TEMOTO_INFO_STREAM("loading " << robot_name);
  rmi_.loadRobot(robot_name);
  TEMOTO_INFO_STREAM(robot_name << " initialized");

  geometry_msgs::PoseStamped target_pose1;  
  target_pose1.pose = rmi_.getEndEffPose(robot_name);
  TEMOTO_INFO_STREAM("EEF pose: " << std::endl << target_pose1.pose);

  /*
   * Move the arm to the grabbing pose
   */
  target_pose1.pose.position.x = 0.217135;
  target_pose1.pose.position.y = -0.159907;
  target_pose1.pose.position.z = 0.434484;

  target_pose1.pose.orientation.x = 0.999546;
  target_pose1.pose.orientation.y = -0.0201953;
  target_pose1.pose.orientation.z = -0.00962823;
  target_pose1.pose.orientation.w = 0.0201746;

  rmi_.planManipulation(robot_name, "xarm7", target_pose1);  
  rmi_.executePlan(robot_name);
  ros::Duration(1).sleep();

  // Grab the object
  rmi_.controlGripperPosition(robot_name, 60);
  ros::Duration(1).sleep();

  /*
   * Move the arm to the object dropping pose
   */
  target_pose1.pose.position.x = 0.670029;
  target_pose1.pose.position.y = -0.00239678;
  target_pose1.pose.position.z = 0.384358;

  target_pose1.pose.orientation.x = 0.999478;
  target_pose1.pose.orientation.y = -0.00292073;
  target_pose1.pose.orientation.z = 0.0295777;
  target_pose1.pose.orientation.w = 0.0126963;

  rmi_.planManipulation(robot_name, "xarm7", target_pose1);  
  rmi_.executePlan(robot_name);
  ros::Duration(1).sleep();

  // Realease the object
  rmi_.controlGripperPosition(robot_name, 100);
  ros::Duration(1).sleep();

  /*
   * Move the arm to the home pose
   */
  target_pose1.pose.position.x = 0.27339;
  target_pose1.pose.position.y = -0.0100824;
  target_pose1.pose.position.z = 0.579062;

  target_pose1.pose.orientation.x = -0.999896;
  target_pose1.pose.orientation.y = -0.00694897;
  target_pose1.pose.orientation.z = -0.0117531;
  target_pose1.pose.orientation.w = 0.00464684;

  rmi_.planManipulation(robot_name, "xarm7", target_pose1);  
  rmi_.executePlan(robot_name);
  ros::Duration(1).sleep();
}

// Destructor
~TaXarm7ManipulateObject()
{
  TEMOTO_INFO("Action instance destructed");
}

robot_manager::RobotManagerInterface<TaXarm7ManipulateObject> rmi_;

}; // TaXarm7ManipulateObject class

/* REQUIRED BY CLASS LOADER */
CLASS_LOADER_REGISTER_CLASS(TaXarm7ManipulateObject, ActionBase);
