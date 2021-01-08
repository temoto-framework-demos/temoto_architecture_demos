#include "ros/ros.h"
#include <temoto_robot_manager/GripperControl.h>
#include <control_msgs/GripperCommandAction.h>
#include <xarm_driver.h>



bool gripperCb (temoto_robot_manager::GripperControl::Request& req, 
		temoto_robot_manager::GripperControl::Response& res)
{
	ros::NodeHandle nh;
	ROS_INFO("xArm gripper request: x=%ld", (long int)req.position);
  	ros::ServiceClient gripper_config_client_2 = nh.serviceClient<xarm_msgs::GripperMove>("gripper_move");

	if (req.position<0)
	{
		req.position=0;	
	}

	else if (req.position>100)
	{
		req.position=100;	
	}
	
	ROS_INFO("sending request");
	xarm_msgs::GripperMove srv2;
  	srv2.request.pulse_pos = (req.position * 850)/100;
  	
	if (gripper_config_client_2.call(srv2))
	{
		ROS_INFO("The gripper moved");
		return true;
	}
	else
	{
		ROS_INFO("The server did not come up");
		return true;
	}
	ROS_INFO("end");
	//return true;
}


int main(int argc, char **argv)
{
	ros::init(argc, argv, "xArm_gripper_controller");
	ros::NodeHandle nh;
	ros::ServiceServer gripper_server = nh.advertiseService("gripper_control", gripperCb);
	ros::ServiceClient gripper_config_client_0 = nh.serviceClient<xarm_msgs::GripperState>("gripper_state");
  	ros::ServiceClient gripper_config_client = nh.serviceClient<xarm_msgs::GripperConfig>("gripper_config");
	xarm_msgs::GripperState srv_0;
  	gripper_config_client_0.call(srv_0);
	xarm_msgs::GripperConfig srv_1;
  	srv_1.request.pulse_vel = 1500;
  	gripper_config_client.call(srv_1);
	ros::spin();

	return 0;
}












