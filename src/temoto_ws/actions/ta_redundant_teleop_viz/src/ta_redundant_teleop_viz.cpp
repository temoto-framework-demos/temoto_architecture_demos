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
#include "temoto_er_manager/temoto_er_manager_interface.h"
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
  ermi_.initialize(this);
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
    + robot_config["navigation"]["driver"]["cmd_vel_mux"].as<std::string>();
  TEMOTO_INFO_STREAM("cmd_vel topic of '" << robot_name_ << "' is '" << robot_cmd_vel_topic << "'");

  std::string robot_description_name = robot_config["robot_absolute_namespace"].as<std::string>()
    + "/robot_description";
  TEMOTO_INFO_STREAM("robot_description of '" << robot_name_ << "' is '" << robot_description_name << "'");

  /*
   * Load a joystick for controlling the robot
   */
  ComponentTopicsReq requested_topics_joystick;
  requested_topics_joystick.addOutputTopic("cmd_vel", robot_cmd_vel_topic);
  cmi_.startComponent("joystick_twist", requested_topics_joystick);

  /*
   * Load a 3D lidar for teleoperation feedback
   */
  ComponentTopicsReq requested_topics_3dl;
  temoto_component_manager::LoadComponent load_component_srv_msg_3dl;
  std::string sensor_name_3dl = "3d_lidar";
  lidar_data_topic_3d_ = "/" + temoto_core::common::getTemotoNamespace() + "/teleoperation_feedback/lidar_data_3d";

  TEMOTO_INFO_STREAM("Starting the " << sensor_name_3dl << " component ...");
  requested_topics_3dl.addOutputTopic("lidar_data_3d", lidar_data_topic_3d_);
  load_component_srv_msg_3dl.request.component_type = sensor_name_3dl;
  load_component_srv_msg_3dl.request.output_topics = requested_topics_3dl.outputTopicsAsKeyValues();

  ComponentTopicsRes responded_topics_3dl = cmi_.startComponent(load_component_srv_msg_3dl, robot_name_);
  lidar_data_topic_3d_ = responded_topics_3dl.getOutputTopic("lidar_data_3d");
  TEMOTO_INFO_STREAM("Got " << sensor_name_3dl <<  " data on topic '" << lidar_data_topic_3d_ << "'");

  /*
   * Show the sensor data and the robot in RViz
   */ 
  robot_description_rviz_config_ = "{Robot Description: " + robot_description_name + "}";
  omi_.showInRviz("robot_model", "", robot_description_rviz_config_);

  lidar_3d_rviz_config_ = "{Topic: " + lidar_data_topic_3d_ + ", " + "Queue Size: 1}";
  omi_.showInRviz("depth image", lidar_data_topic_3d_, lidar_3d_rviz_config_);
  
  // Start a static tf publisher which links the "world" frame with "base_link" frame
  //ermi_.loadResource("tf", "static_transform_publisher", "0 0 0 0 0 0 /world /base_link 50");
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
    // omi_.hideInRviz("laser scan", lidar_data_topic_2d_);
    // ComponentTopicsRes responded_topics = cmi_.startComponent("3d_lidar");
    // lidar_data_topic_3d_ = responded_topics.getOutputTopic("lidar_data_3d");
    // omi_.showInRviz("depth image", lidar_data_topic_3d_);
  }
  else if (comp_srv_msg.request.component_type == "3d_lidar")
  {
    // Remove the 3D lidar data visualization plugin
    omi_.hideInRviz("depth image", lidar_data_topic_3d_, lidar_3d_rviz_config_);

    /*
     * Load a 3D camera for teleoperation feedback
     */
    ComponentTopicsReq requested_topics;
    temoto_component_manager::LoadComponent load_component_srv_msg;
    std::string sensor_name = "3d_camera";
    camera_data_topic_3d_ = "/" + temoto_core::common::getTemotoNamespace() + "/teleoperation_feedback/camera_data_3d";

    TEMOTO_INFO_STREAM("Starting the " << sensor_name << " component ...");
    requested_topics.addOutputTopic("camera_data_3d", camera_data_topic_3d_);
    load_component_srv_msg.request.component_type = sensor_name;
    load_component_srv_msg.request.output_topics = requested_topics.outputTopicsAsKeyValues();

    ComponentTopicsRes responded_topics = cmi_.startComponent(load_component_srv_msg, robot_name_);
    camera_data_topic_3d_ = responded_topics.getOutputTopic("camera_data_3d");
    TEMOTO_INFO_STREAM("Got " << sensor_name <<  " data on topic '" << camera_data_topic_3d_ << "'");

    // Show 3D camera data in RViz
    omi_.showInRviz("depth image", camera_data_topic_3d_);
  }
  else if (comp_srv_msg.request.component_type == "3d_camera")
  {
    // Remove the 3D camera data visualization plugin
    omi_.hideInRviz("depth image", camera_data_topic_3d_);

    /*
     * Load a 2D lidar for teleoperation feedback
     */
    ComponentTopicsReq requested_topics;
    temoto_component_manager::LoadComponent load_component_srv_msg;
    std::string sensor_name = "2d_lidar";
    lidar_data_topic_2d_ = "/" + temoto_core::common::getTemotoNamespace() + "/teleoperation_feedback/lidar_data_2d";

    TEMOTO_INFO_STREAM("Starting the " << sensor_name << " component ...");
    requested_topics.addOutputTopic("lidar_data_2d", lidar_data_topic_2d_);
    load_component_srv_msg.request.component_type = sensor_name;
    load_component_srv_msg.request.output_topics = requested_topics.outputTopicsAsKeyValues();

    ComponentTopicsRes responded_topics = cmi_.startComponent(load_component_srv_msg, robot_name_);
    std::string lidar_data_topic_2d_res = responded_topics.getOutputTopic("lidar_data_2d");
    TEMOTO_INFO_STREAM("Got " << sensor_name <<  " data on topic '" << lidar_data_topic_2d_ << "'");

    // Show 2D lidar data in RViz
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
temoto_er_manager::ERManagerInterface<TaRedundantTeleopViz> ermi_;
robot_manager::RobotManagerInterface<TaRedundantTeleopViz> rmi_;

std::string lidar_data_topic_2d_;
std::string lidar_data_topic_3d_;
std::string camera_data_topic_3d_;

std::string lidar_3d_rviz_config_;
std::string lidar_2d_rviz_config_;
std::string camera_3d_rviz_config_;
std::string robot_description_rviz_config_;

std::string robot_name_ = "jackal";

}; // TaRedundantTeleopViz class

/* REQUIRED BY CLASS LOADER */
CLASS_LOADER_REGISTER_CLASS(TaRedundantTeleopViz, ActionBase);
