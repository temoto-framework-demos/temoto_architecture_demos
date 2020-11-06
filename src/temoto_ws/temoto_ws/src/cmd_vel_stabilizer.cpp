#include <string>
#include <mutex>
#include "ros/ros.h"
#include "geometry_msgs/Twist.h"

class CmdVelStabilizer
{
public:
  CmdVelStabilizer(const std::string& cmd_vel_topic_in
  , const std::string& cmd_vel_topic_out
  , float publish_frequency)
  : cmd_vel_topic_in_(cmd_vel_topic_in)
  , cmd_vel_topic_out_(cmd_vel_topic_out)
  , publish_frequency_(publish_frequency)
  , deadman_duration_(ros::Duration(3))
  {
    // Initialize the cmd vel publisher
    cmd_vel_pub_ = nh_.advertise<geometry_msgs::Twist>(cmd_vel_topic_out_, 1);

    // Initialize the cmd vel subscriber
    cmd_vel_sub_ = nh_.subscribe(cmd_vel_topic_in_, 1, &CmdVelStabilizer::cmdVelInCallback, this);

    // Initialize the timer
    cmd_vel_pub_timer_ = nh_.createTimer(ros::Duration(1.0/publish_frequency), &CmdVelStabilizer::timerCallback, this);

    ROS_INFO_STREAM("cmd_vel stabilizer is initialized");
  }

private:
  void timerCallback(const ros::TimerEvent&)
  {
    std::lock_guard<std::mutex> guard(cmd_vel_msg_mutex_);

    if ((ros::Time::now() - latest_cmd_vel_msg_time_) < deadman_duration_)
    {
      cmd_vel_pub_.publish(latest_cmd_vel_msg_);
    }
  }

  void cmdVelInCallback(const geometry_msgs::Twist& msg)
  {
    std::lock_guard<std::mutex> guard(cmd_vel_msg_mutex_);
    latest_cmd_vel_msg_time_ = ros::Time::now();
    latest_cmd_vel_msg_ = msg;
  }

  ros::NodeHandle nh_;
  ros::Subscriber cmd_vel_sub_;
  ros::Publisher cmd_vel_pub_;
  ros::Timer cmd_vel_pub_timer_;

  std::mutex cmd_vel_msg_mutex_;
  geometry_msgs::Twist latest_cmd_vel_msg_;
  ros::Time latest_cmd_vel_msg_time_;
  ros::Duration deadman_duration_;

  std::string cmd_vel_topic_in_ = "cmd_vel_in";
  std::string cmd_vel_topic_out_ = "cmd_vel_out";
  float publish_frequency_ = 25;
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "cmd_vel_stabilizer");
  ros::NodeHandle nh("~");

  std::string cmd_vel_in;
  std::string cmd_vel_out;
  float frequency;

  // Get the parameters
  if (!nh.getParam("cmd_vel_in", cmd_vel_in))
  {
    ROS_ERROR_STREAM("Could not get the 'cmd_vel_in' parameter");
    return 1;
  }

  if (!nh.getParam("cmd_vel_out", cmd_vel_out))
  {
    ROS_ERROR_STREAM("Could not get the 'cmd_vel_out' parameter");
    return 1;
  }

  if (!nh.getParam("frequency", frequency))
  {
    ROS_ERROR_STREAM("Could not get the 'frequecy' parameter");
    return 1;
  }

  // Set up the cmd_vel stabiliser
  CmdVelStabilizer cmd_vel_stabilizer(cmd_vel_in, cmd_vel_out, frequency);

  ros::spin();

  return 0;
}