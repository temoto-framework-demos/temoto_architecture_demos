
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
#include "ta_delay/temoto_action.h"
#include "ta_delay/macros.h"

/* 
 * ACTION IMPLEMENTATION of TaDelay 
 */
class TaDelay : public TemotoAction
{
public:

// Constructor. REQUIRED BY TEMOTO
TaDelay()
{
  std::cout << __func__ << " constructed\n";
}

// REQUIRED BY TEMOTO
void executeTemotoAction()
{
  // Input parameters
  double in_param_duration = GET_PARAMETER("duration", double);
  TEMOTO_INFO_STREAM("Waiting for " << in_param_duration << " seconds ...");
  ros::Duration(in_param_duration).sleep();
  TEMOTO_INFO_STREAM("Done");
}

// Destructor
~TaDelay()
{
  TEMOTO_INFO("Action instance destructed");
}

}; // TaDelay class

/* REQUIRED BY CLASS LOADER */
CLASS_LOADER_REGISTER_CLASS(TaDelay, ActionBase);
