
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
#include "ta_follow_ar_tag/temoto_action.h"
#include "ta_follow_ar_tag/macros.h"

#include "temoto_component_manager/component_manager_interface.h"
#include "temoto_robot_manager/robot_manager_interface.h"

/* 
 * ACTION IMPLEMENTATION of TaFollowArTag 
 */
class TaFollowArTag : public TemotoAction
{
public:

// Constructor. REQUIRED BY TEMOTO
TaFollowArTag()
{
  std::cout << __func__ << " constructed\n";
}

// REQUIRED BY TEMOTO
void executeTemotoAction()
{
  cmi_.initialize(this);
  rmi_.initialize(this);

  /*
   * Load the robot
   */
  TEMOTO_INFO_STREAM("loading " << robot_name_);
  rmi_.loadRobot(robot_name_);
  TEMOTO_INFO_STREAM(robot_name_ << " initialized");

  TEMOTO_INFO_STREAM("trying to get config of '" << robot_name_ << "' ...");
  YAML::Node robot_config = rmi_.getRobotConfig(robot_name_);
  TEMOTO_INFO_STREAM("Config of robot '" << robot_name_ << "': " << robot_config);

  std::string robot_cmd_vel_topic = robot_config["robot_absolute_namespace"].as<std::string>() + "/"
    + robot_config["navigation"]["driver"]["cmd_vel_topic"].as<std::string>();
  TEMOTO_INFO_STREAM("cmd_vel topic of '" << robot_name_ << "' is '" << robot_cmd_vel_topic << "'");

  /*
   * Start the AR tag detection pipe
   */
  TEMOTO_INFO_STREAM("Starting the ar_tag_tracking pipe ...");
  temoto_core::TopicContainer pipe_topics = cmi_.startPipe("ar_tag_tracking");
  std::string ar_tag_data_topic = pipe_topics.getOutputTopic("ar_tag_data");
  TEMOTO_INFO_STREAM("Got AR tag data on topic '" << ar_tag_data_topic << "'");

  /*
   * Start the AR tag tracker, which makes the robot follow the tag
   */
  TEMOTO_INFO_STREAM("Starting the 'ar_tag_tracker' ...");
  ComponentTopicsReq requested_topics;
  requested_topics.addInputTopic("ar_tag_data", ar_tag_data_topic);
  requested_topics.addOutputTopic("cmd_vel", robot_cmd_vel_topic);
  cmi_.startComponent("ar_cmd_vel_follower", requested_topics);
}

// Destructor
~TaFollowArTag()
{
  TEMOTO_INFO("Action instance destructed");
}

// Create manager interface objects
temoto_component_manager::ComponentManagerInterface<TaFollowArTag> cmi_;
robot_manager::RobotManagerInterface<TaFollowArTag> rmi_;

std::string robot_name_ = "clearbot";

}; // TaFollowArTag class

/* REQUIRED BY CLASS LOADER */
CLASS_LOADER_REGISTER_CLASS(TaFollowArTag, ActionBase);
