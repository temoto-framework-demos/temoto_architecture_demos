
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
#include "temoto_action_engine/umrf_json_converter.h"
#include "temoto_action_engine/UmrfGraph.h"

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

  /*
   * Load the robot 1 and get its configuration
   */ 
  TEMOTO_INFO_STREAM("loading " << robot_name_1_);
  rmi_.loadRobot(robot_name_1_);
  TEMOTO_INFO_STREAM(robot_name_1_ << " initialized");

  TEMOTO_INFO_STREAM("trying to get config of '" << robot_name_1_ << "' ...");
  YAML::Node robot_1_config = rmi_.getRobotConfig(robot_name_1_);
  TEMOTO_INFO_STREAM("Config of robot '" << robot_name_1_ << "': " << robot_1_config);

  /*
   * Load the robot 2 and get its configuration
   */ 
  TEMOTO_INFO_STREAM("loading " << robot_name_2_);
  rmi_.loadRobot(robot_name_2_);
  TEMOTO_INFO_STREAM(robot_name_2_ << " initialized");

  TEMOTO_INFO_STREAM("trying to get config of '" << robot_name_2_ << "' ...");
  YAML::Node robot_2_config = rmi_.getRobotConfig(robot_name_2_);
  TEMOTO_INFO_STREAM("Config of robot '" << robot_name_2_ << "': " << robot_2_config);

  std::string robot_1_cmd_vel_topic = robot_1_config["robot_absolute_namespace"].as<std::string>() + "/"
    + robot_1_config["navigation"]["driver"]["cmd_vel_topic"].as<std::string>();
  TEMOTO_INFO_STREAM("cmd_vel topic of '" << robot_name_1_ << "' is '" << robot_1_cmd_vel_topic << "'");

  /*
   * Load a joystick for controlling the robot
   */
  ComponentTopicsReq requested_topics_joystick;
  requested_topics_joystick.addOutputTopic("cmd_vel", robot_1_cmd_vel_topic);
  cmi_.startComponent("joystick_twist", requested_topics_joystick);

  /*
   * Load a camera for teleoperation feedback and show the feedback in rviz
   */
  ComponentTopicsReq requested_topics;
  temoto_component_manager::LoadComponent load_component_srv_msg;
  sensor_topic_ = "/" + temoto_core::common::getTemotoNamespace() + "/teleoperation_feedback/camera_data/compressed";
  sensor_info_topic_ = "/" + temoto_core::common::getTemotoNamespace() + "/teleoperation_feedback/camera_info";

  TEMOTO_INFO_STREAM("Starting the " << sensor_name_ << " component ...");
  if (sensor_name_ == "2d_camera")
  {
    requested_topics.addOutputTopic("camera_data_compressed", sensor_topic_);
    requested_topics.addOutputTopic("camera_info", sensor_info_topic_);
    load_component_srv_msg.request.component_type = sensor_name_;
    load_component_srv_msg.request.output_topics = requested_topics.outputTopicsAsKeyValues();

    ComponentTopicsRes responded_topics = cmi_.startComponent(load_component_srv_msg, robot_name_1_);
    std::string sensor_topic_res = responded_topics.getOutputTopic("camera_data_compressed");
    TEMOTO_INFO_STREAM("Got " << sensor_name_ <<  " data on topic '" << sensor_topic_res << "'");

    omi_.showInRviz("compressed_image", sensor_topic_);
  }
}

/**
 * @brief This function is invoked by Component Manager Interface whenever
 * any allocated component should fail
 * 
 * @param comp_srv_msg The message that was used to load the component
 */
void componentStatusCb(const temoto_component_manager::LoadComponent& comp_srv_msg)
{
  TEMOTO_WARN_STREAM("Received a status message:\n" << comp_srv_msg.request);

  /*
   * Try to load the same component on a different robot
   */ 
  TEMOTO_INFO_STREAM("Starting the " << sensor_name_ << " component on " << robot_name_2_ << " ...");

  ComponentTopicsReq requested_topics;
  temoto_component_manager::LoadComponent load_component_srv_msg;
  requested_topics.addOutputTopic("camera_data_compressed", sensor_topic_);
  requested_topics.addOutputTopic("camera_info", sensor_info_topic_);
  load_component_srv_msg.request.component_type = sensor_name_;
  load_component_srv_msg.request.output_topics = requested_topics.outputTopicsAsKeyValues();
  
  ComponentTopicsRes responded_topics = cmi_.startComponent(load_component_srv_msg, robot_name_2_);
  std::string sensor_topic_res = responded_topics.getOutputTopic("camera_data_compressed");
  TEMOTO_INFO_STREAM("Got " << sensor_name_ <<  " data on topic '" << sensor_topic_res << "'");

  /*
   * Make the second robot escort the robot that had the component failure
   */
  Umrf umrf;
  umrf.setName("TaFollowArTag");
  umrf.setSuffix(0);
  umrf.setEffect("asynchronous");

  // ActionParameters ap;
  // ap.setParameter("ar_tag_id", "number", boost::any_cast<double>(0.0));
  // umrf.setInputParameters(ap);

  temoto_action_engine::UmrfGraph umrf_graph_msg;
  umrf_graph_msg.graph_name = "escort_jackal";
  UmrfGraph ug(umrf_graph_msg.graph_name, std::vector<Umrf>{umrf}, false);
  umrf_graph_msg.umrf_graph_json = umrf_json_converter::toUmrfGraphJsonStr(ug);
  umrf_graph_msg.name_match_required = true;
  umrf_graph_msg.targets.push_back(robot_name_2_);

  publishUmrfGraph(umrf_graph_msg);
}

void publishUmrfGraph(const temoto_action_engine::UmrfGraph& umrf_graph_msg)
{
  ros::NodeHandle nh;
  ros::Publisher umrf_graph_pub = nh.advertise<temoto_action_engine::UmrfGraph>("/umrf_graph_topic", 1);

  /*
   * Wait until there is somebody to publish the message to
   */
  TEMOTO_INFO_STREAM("Waiting for subscribers ...");
  while (umrf_graph_pub.getNumSubscribers() <= 0 && ros::ok())
  {
    ros::Duration(0.1).sleep();
  }

  /*
   * Publish the UMRF JSON graph
   */
  TEMOTO_INFO_STREAM("Publishing the UMRF Graph message ...");
  if (ros::ok())
  {
    // Sleep for some time, so that "all" possible pub/sub connections are made
    ros::Duration(3).sleep();
    umrf_graph_pub.publish(umrf_graph_msg);
  }

  TEMOTO_INFO_STREAM("UMRF Graph message published, shutting down.");
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
std::string sensor_info_topic_;
std::string sensor_name_;
std::string robot_name_1_;
std::string robot_name_2_;

}; // TaRemoteSensorControl class

/* REQUIRED BY CLASS LOADER */
CLASS_LOADER_REGISTER_CLASS(TaRemoteSensorControl, ActionBase);
