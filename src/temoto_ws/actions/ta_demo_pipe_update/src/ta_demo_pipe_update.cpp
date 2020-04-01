
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
#include "ta_demo_pipe_update/temoto_action.h"
#include "ta_demo_pipe_update/macros.h"

#include "temoto_component_manager/component_manager_interface.h"

/* 
 * ACTION IMPLEMENTATION of TaDemoPipeUpdate 
 */
class TaDemoPipeUpdate : public TemotoAction
{
public:

// Constructor. REQUIRED BY TEMOTO
TaDemoPipeUpdate()
{
  std::cout << __func__ << " constructed\n";
}

// REQUIRED BY TEMOTO
void executeTemotoAction()
{
  // Initialize the component manager interface
  cmi_.initialize(this);

  TEMOTO_INFO("Registering a pipe status callback ...");
  cmi_.registerPipeStatusCallback(&TaDemoPipeUpdate::pipeStatusCb);
  cmi_.registerPipeUpdateCallback(&TaDemoPipeUpdate::pipeUpdateCb);

  // Start the pipe and get the output topics of the last pipe segment
  TEMOTO_INFO("Starting the processed_pointcloud pipe ...");
  temoto_core::TopicContainer pipe_topics = cmi_.startPipe("processed_pointcloud");

  // Get the name of the topic where camera feed is published
  std::string ppc_data_topic = pipe_topics.getOutputTopic("camera_data_3d");

  // Debug information
  TEMOTO_INFO_STREAM("Got processsed_pointcloud on topic '" << ppc_data_topic << "'");

  /*
   * Now your pipe is set. Create a subscriber to ppc_data_topic or do whatever you need to do with the topic
   */
}

/**
 * @brief A custom pipe failure recovery routine. You can implement whatever you like
 * in here, such as logging, system rollbacking, starting an alternative pipe ...
 */
void pipeStatusCb(const temoto_component_manager::LoadPipe& pipe_srv_msg)
{
  TEMOTO_WARN_STREAM("Received a status message:\n" << pipe_srv_msg.request);
  TEMOTO_INFO_STREAM("Starting the pipe again ...");
  cmi_.startPipe(pipe_srv_msg.request.pipe_category);
}

void pipeUpdateCb(const temoto_component_manager::LoadPipe& pipe_srv_msg)
{
  try
  {
    TEMOTO_INFO_STREAM("Received a pipe update message:\n" << pipe_srv_msg.request);
    TEMOTO_INFO_STREAM("Reloading the pipe again ...");
    cmi_.reloadPipe(pipe_srv_msg.request.pipe_category);
  }
  catch (temoto_core::error::ErrorStack& error_stack)
  {
    TEMOTO_INFO_STREAM(error_stack);
  }
}

// Destructor
~TaDemoPipeUpdate()
{
  TEMOTO_INFO("Action instance destructed");
}

// Create component manager interface object to access the component manager
temoto_component_manager::ComponentManagerInterface<TaDemoPipeUpdate> cmi_;

}; // TaDemoPipeUpdate class

/* REQUIRED BY CLASS LOADER */
CLASS_LOADER_REGISTER_CLASS(TaDemoPipeUpdate, ActionBase);
