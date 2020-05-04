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

  cmi_.registerComponentStatusCallback(&TaRedundantTeleopViz::componentStatusCb);

  // A structure for containing component topic related information
  ComponentTopicsReq requested_topics;

  // Specify what kind of topics the component should output. These are the same as
  // outlined in the components.yaml file
  requested_topics.addOutputTopicType("lidar_data_2d");

  // Debug information
  TEMOTO_INFO("Starting the 2d_lidar component ...");

  /*
   * Invoke the "startComponent" method. This method requires the type of the sensor (2d_camera)
   * and will output a structure that maintains the topic names which were received based
   * on the request
   */
  ComponentTopicsRes responded_topics = cmi_.startComponent("2d_lidar", requested_topics);

  // Get the name of the topic where camera feed is published
  lidar_data_topic_2d = responded_topics.getOutputTopic("lidar_data_2d");

  // Debug information
  TEMOTO_INFO_STREAM("Got 2d_lidar data on topic '" << lidar_data_topic_2d << "'");

  // Show the lidar output in rviz
  omi_.showInRviz("robot_model");
  omi_.showInRviz("laser scan", lidar_data_topic_2d);
  ermi_.loadResource("tf", "static_transform_publisher", "0 0 0 0 0 0 /world /odom 50");
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
    omi_.hideInRviz("laser scan", lidar_data_topic_2d);
    ComponentTopicsRes responded_topics = cmi_.startComponent("3d_lidar");
    omi_.showInRviz("depth image", responded_topics.getOutputTopic("lidar_data_3d"));
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

std::string lidar_data_topic_2d;

}; // TaRedundantTeleopViz class

/* REQUIRED BY CLASS LOADER */
CLASS_LOADER_REGISTER_CLASS(TaRedundantTeleopViz, ActionBase);
