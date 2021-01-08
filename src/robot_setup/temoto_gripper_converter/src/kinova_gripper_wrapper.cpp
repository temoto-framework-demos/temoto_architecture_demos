#include "ros/ros.h"
#include <temoto_robot_manager/GripperControl.h>
#include <actionlib/client/simple_action_client.h>
#include <control_msgs/GripperCommandAction.h>
#include <kinova_msgs/SetFingersPositionAction.h>


std::string kinova_robotType = "m1n6s300_";
const double FINGER_MAX = 6400;


bool gripperCb (temoto_robot_manager::GripperControl::Request& req, 
		temoto_robot_manager::GripperControl::Response& res)
{
	ROS_INFO("Kinova gripper request: x=%ld", (long int)req.position);
	std::string action_address = "m1n6s300_driver/fingers_action/finger_positions";	
	typedef actionlib::SimpleActionClient<kinova_msgs::SetFingersPositionAction> finger_client;	
	finger_client client(action_address, true);    
	kinova_msgs::SetFingersPositionGoal goal;		
	if (req.position<0)
	{
	req.position=0;	
	}
	goal.fingers.finger1 = (100 - float(req.position))/100 * FINGER_MAX;
	goal.fingers.finger2 = (100 - float(req.position))/100 * FINGER_MAX;
	goal.fingers.finger3 = (100 - float(req.position))/100 * FINGER_MAX;		

	if (!client.waitForServer(ros::Duration(5.0)))
    	{
      	ROS_INFO("The kinova gripper server did not come up");
    	}
	else
	{
		ROS_INFO("sending goal");
		client.sendGoal(goal);
		ROS_INFO("waitForResult");
		//client.waitForResult();
		if (client.waitForResult(ros::Duration(5.0)))
		{
			client.getResult();
			return true;
		}
		else
		{
			ROS_INFO("The kinova gripper server did not come up");
			client.cancelAllGoals();
			return false;
		}
	}
	ROS_INFO("end");
	//return true;
}


int main(int argc, char **argv)
{
	ros::init(argc, argv, "kinova_gripper_wrapper");
	ros::NodeHandle nh;
	ros::ServiceServer gripper_server = nh.advertiseService("gripper_control", gripperCb);
	ros::spin();
	return 0;
}












