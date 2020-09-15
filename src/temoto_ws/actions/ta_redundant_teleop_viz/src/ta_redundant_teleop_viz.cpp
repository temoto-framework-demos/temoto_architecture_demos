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
#include "ta_redundant_teleop_viz/temoto_action.h"
#include "ta_redundant_teleop_viz/macros.h"
#include "temoto_component_manager/component_manager_interface.h"
#include "temoto_output_manager/output_manager_interface.h"
//#include "temoto_er_manager/temoto_er_manager_interface.h"
#include "temoto_robot_manager/robot_manager_interface.h"

/* 
 * ACTION IMPLEMENTATION of TaRedundantTeleopViz 
 */
class TaRedundantTeleopViz : public TemotoAction
{
public:

// Constructor. REQUIRED BY TEMOTO
TaRedundantTeleopViz()
{
  std::cout << __func__ << " constructed\n";
}

// REQUIRED BY TEMOTO
void executeTemotoAction()
{
  // Initialize the manager interfaces
  cmi_.initialize(this);
  omi_.initialize(this);
  //ermi_.initialize(this);
  rmi_.initialize(this);

  cmi_.registerComponentStatusCallback(&TaRedundantTeleopViz::componentStatusCb);

  /*
   * Load the robot and get its configuration
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
   * Load a joystick for controlling the robot
   */
  ComponentTopicsReq requested_topics_joystick;
  requested_topics_joystick.addOutputTopic("cmd_vel", robot_cmd_vel_topic);
  cmi_.startComponent("joystick_twist", requested_topics_joystick);

  /*
   * Load a lidar for teleoperation feedback and show the feedback in rviz
   */
  ComponentTopicsReq requested_topics;
  temoto_component_manager::LoadComponent load_component_srv_msg;
  std::string sensor_name_ = "2d_lidar";
  std::string lidar_data_topic_2d_ = "/" + temoto_core::common::getTemotoNamespace() + "/teleoperation_feedback/lidar_data_2d";

  TEMOTO_INFO_STREAM("Starting the " << sensor_name_ << " component ...");
  requested_topics.addOutputTopic("lidar_data_2d", lidar_data_topic_2d_);
  load_component_srv_msg.request.component_type = sensor_name_;
  load_component_srv_msg.request.output_topics = requested_topics.outputTopicsAsKeyValues();

  ComponentTopicsRes responded_topics = cmi_.startComponent(load_component_srv_msg, robot_name_);
  std::string lidar_data_topic_2d_res = responded_topics.getOutputTopic("lidar_data_2d");
  TEMOTO_INFO_STREAM("Got " << sensor_name_ <<  " data on topic '" << lidar_data_topic_2d_res << "'");

  omi_.showInRviz("laser scan", lidar_data_topic_2d_);

  // Show the lidar output in rviz
  //omi_.showInRviz("robot_model");
  //omi_.showInRviz("laser scan", lidar_data_topic_2d_);
  //ermi_.loadResource("tf", "static_transform_publisher", "0 0 0 0 0 0 /world /odom 50");
}

/**
 * @brief A custom component failure recovery routine. You can implement whatever you like
 * in here, such as logging, system rollbacking, starting an alternative component ...
 */
void componentStatusCb(const temoto_component_manager::LoadComponent& comp_srv_msg)
{
  TEMOTO_WARN_STREAM("Received a status message:\n" << comp_srv_msg.request);

  if (comp_srv_msg.request.component_type == "2d_lidar")
  {
    // If the 2D LIDAR broke down, load the 3D LIDAR
    omi_.hideInRviz("laser scan", lidar_data_topic_2d_);
    ComponentTopicsRes responded_topics = cmi_.startComponent("3d_lidar");
    lidar_data_topic_3d_ = responded_topics.getOutputTopic("lidar_data_3d");
    omi_.showInRviz("depth image", lidar_data_topic_3d_);
  }
  else if (comp_srv_msg.request.component_type == "3d_lidar")
  {
    // If the 3D LIDAR broke down, load the 3D camera
    omi_.hideInRviz("depth image", lidar_data_topic_3d_);
    ComponentTopicsRes responded_topics = cmi_.startComponent("2d_lidar");
    lidar_data_topic_2d_ = responded_topics.getOutputTopic("lidar_data_2d");
    omi_.showInRviz("laser scan", lidar_data_topic_2d_);
  }
  else if (comp_srv_msg.request.component_type == "3d_camera")
  {
    // If the 3D camera broke down, load the 2D LIDAR
    omi_.hideInRviz("depth image", camera_topic_2d_);
    ComponentTopicsRes responded_topics = cmi_.startComponent("2d_lidar");
    lidar_data_topic_2d_ = responded_topics.getOutputTopic("lidar_data_2d");
    omi_.showInRviz("laser scan", lidar_data_topic_2d_);
  }
}


// Destructor
~TaRedundantTeleopViz()
{
  TEMOTO_INFO("Action instance destructed");
}

// Create manager interface objects
temoto_component_manager::ComponentManagerInterface<TaRedundantTeleopViz> cmi_;
temoto_output_manager::OutputManagerInterface<TaRedundantTeleopViz> omi_;
//temoto_er_manager::ERManagerInterface<TaRedundantTeleopViz> ermi_;
robot_manager::RobotManagerInterface<TaRedundantTeleopViz> rmi_;

std::string lidar_data_topic_2d_;
std::string lidar_data_topic_3d_;
std::string camera_topic_2d_;

std::string robot_name_ = "jackal";

}; // TaRedundantTeleopViz class

/* REQUIRED BY CLASS LOADER */
CLASS_LOADER_REGISTER_CLASS(TaRedundantTeleopViz, ActionBase);
