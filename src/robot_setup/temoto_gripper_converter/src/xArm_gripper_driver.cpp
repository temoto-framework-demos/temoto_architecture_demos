#include "ros/ros.h"
#include <temoto_robot_manager/GripperControl.h>
#include <control_msgs/GripperCommandAction.h>
#include <xarm_driver.h>



int main(int argc, char **argv)
{
	ros::init(argc, argv, "xArm_gripper_driver");	
	ros::NodeHandle nh;
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












