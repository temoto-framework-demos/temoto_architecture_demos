
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
#include "ta_initialize_robot/temoto_action.h"
#include "ta_initialize_robot/macros.h"

#include "temoto_robot_manager/robot_manager_interface.h"

/* 
 * ACTION IMPLEMENTATION of TaInitializeRobot 
 */
class TaInitializeRobot : public TemotoAction
{
public:

// Constructor. REQUIRED BY TEMOTO
TaInitializeRobot()
{
  std::cout << __func__ << " constructed\n";
}

// REQUIRED BY TEMOTO
void executeTemotoAction()
{
  // Input parameters
  std::string robot_name = GET_PARAMETER("robot_name", std::string);

  rmi_.initialize(this);

  TEMOTO_INFO_STREAM("loading " << robot_name);
  rmi_.loadRobot(robot_name);
  TEMOTO_INFO_STREAM(robot_name << " initialized");

  TEMOTO_INFO_STREAM("trying to get config of '" << robot_name << "' ...");
  YAML::Node robot_config = rmi_.getRobotConfig(robot_name);
  TEMOTO_INFO_STREAM("Config of robot '" << robot_name << "': " << robot_config);
}

// Destructor
~TaInitializeRobot()
{
  TEMOTO_INFO("Action instance destructed");
}

robot_manager::RobotManagerInterface<TaInitializeRobot> rmi_;

}; // TaInitializeRobot class

/* REQUIRED BY CLASS LOADER */
CLASS_LOADER_REGISTER_CLASS(TaInitializeRobot, ActionBase);
