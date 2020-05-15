
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
#include "ta_remote_sensor_control/temoto_action.h"
#include "ta_remote_sensor_control/macros.h"

#include "temoto_component_manager/component_manager_interface.h"
#include "temoto_output_manager/output_manager_interface.h"
#include "temoto_robot_manager/robot_manager_interface.h"

/* 
 * ACTION IMPLEMENTATION of TaRemoteSensorControl 
 */
class TaRemoteSensorControl : public TemotoAction
{
public:

// Constructor. REQUIRED BY TEMOTO
TaRemoteSensorControl()
{
  std::cout << __func__ << " constructed\n";
}

// REQUIRED BY TEMOTO
void executeTemotoAction()
{
  // Input parameters
  sensor_name_ = GET_PARAMETER("sensor_name", std::string);
  robot_name_1_ = GET_PARAMETER("robot_name_1", std::string);
  robot_name_2_ = GET_PARAMETER("robot_name_2", std::string);

   // Initialize the manager interfaces
  cmi_.initialize(this);
  omi_.initialize(this);
  rmi_.initialize(this);
  cmi_.registerComponentStatusCallback(&TaRemoteSensorControl::componentStatusCb);

  // Load the robot and get its configuration
  // TEMOTO_INFO_STREAM("loading " << robot_name_1_);
  // rmi_.loadRobot(robot_name_1_);
  // TEMOTO_INFO_STREAM(robot_name_1_ << " initialized");

  TEMOTO_INFO_STREAM("trying to get config of '" << robot_name_1_ << "' ...");
  YAML::Node robot_1_config = rmi_.getRobotConfig(robot_name_1_);
  TEMOTO_INFO_STREAM("got it");
  TEMOTO_INFO_STREAM("Config of robot '" << robot_name_1_ << "': " << robot_1_config);

  std::string robot_1_cmd_vel_topic = robot_1_config["robot_absolute_namespace"].as<std::string>() + "/"
    + robot_1_config["navigation"]["driver"]["cmd_vel_topic"].as<std::string>();

  TEMOTO_INFO_STREAM("cmd_vel topic of '" << robot_name_1_ << "' is '" << robot_1_cmd_vel_topic << "'");

  // // A structure for containing component topic related information
  // ComponentTopicsReq requested_topics;
  // temoto_component_manager::LoadComponent load_component_srv_msg;
  // sensor_topic_ = "/" + temoto_core::common::getTemotoNamespace() + "/teleoperation_feedback";

  // TEMOTO_INFO_STREAM("Starting the " << sensor_name_ << " component ...");
  // if (sensor_name_ == "2d_camera")
  // {
  //   requested_topics.addOutputTopic("camera_data_2d", sensor_topic_);
  //   load_component_srv_msg.request.component_type = sensor_name_;
  //   load_component_srv_msg.request.output_topics = requested_topics.outputTopicsAsKeyValues();

  //   ComponentTopicsRes responded_topics = cmi_.startComponent(load_component_srv_msg, robot_name_1_);
  //   std::string sensor_topic_res = responded_topics.getOutputTopic("camera_data_2d");
  //   TEMOTO_INFO_STREAM("Got " << sensor_name_ <<  " data on topic '" << sensor_topic_res << "'");

  //   omi_.showInRviz("image", sensor_topic_);
  // }
}

void componentStatusCb(const temoto_component_manager::LoadComponent& comp_srv_msg)
{
  TEMOTO_WARN_STREAM("Received a status message:\n" << comp_srv_msg.request);
  TEMOTO_INFO_STREAM("Starting the " << sensor_name_ << " component on " << robot_name_2_ << " ...");
  //omi_.hideInRviz("image", sensor_topic_);

  ComponentTopicsReq requested_topics;
  temoto_component_manager::LoadComponent load_component_srv_msg;
  requested_topics.addOutputTopic("camera_data_2d", sensor_topic_);
  load_component_srv_msg.request.component_type = sensor_name_;
  load_component_srv_msg.request.output_topics = requested_topics.outputTopicsAsKeyValues();
  
  ComponentTopicsRes responded_topics = cmi_.startComponent(load_component_srv_msg, robot_name_2_);
  std::string sensor_topic_res = responded_topics.getOutputTopic("camera_data_2d");
  TEMOTO_INFO_STREAM("Got " << sensor_name_ <<  " data on topic '" << sensor_topic_res << "'");
}

// Destructor
~TaRemoteSensorControl()
{
  TEMOTO_INFO("Action instance destructed");
}

// Create manager interface objects
temoto_component_manager::ComponentManagerInterface<TaRemoteSensorControl> cmi_;
temoto_output_manager::OutputManagerInterface<TaRemoteSensorControl> omi_;
robot_manager::RobotManagerInterface<TaRemoteSensorControl> rmi_;

std::string sensor_topic_;
std::string sensor_name_;
std::string robot_name_1_;
std::string robot_name_2_;

}; // TaRemoteSensorControl class

/* REQUIRED BY CLASS LOADER */
CLASS_LOADER_REGISTER_CLASS(TaRemoteSensorControl, ActionBase);
