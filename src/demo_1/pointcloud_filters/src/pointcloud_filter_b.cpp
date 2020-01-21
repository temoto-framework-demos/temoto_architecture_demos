#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/filters/crop_box.h>

ros::Publisher pub;

struct BoxFilterParams
{
  BoxFilterParams(double default_val_in)
  : default_val(default_val_in)
  {}

  double default_val;

  double min_x = -default_val;
  double min_y = -default_val;
  double min_z = -default_val;

  double max_x = default_val;
  double max_y = default_val;
  double max_z = default_val;
};

BoxFilterParams bfp(2.0);

void cloud_cb (const sensor_msgs::PointCloud2ConstPtr& cloud_msg)
{
  // Container for original & filtered data
  pcl::PCLPointCloud2* cloud = new pcl::PCLPointCloud2; 
  pcl::PCLPointCloud2ConstPtr cloudPtr(cloud);
  pcl::PCLPointCloud2 cloud_filtered;

  // Convert to PCL data type
  pcl_conversions::toPCL(*cloud_msg, *cloud);

  // Apply the box filter
  pcl::CropBox<pcl::PCLPointCloud2> boxFilter;
  boxFilter.setMin(Eigen::Vector4f(bfp.min_x, bfp.min_y, bfp.min_z, 1.0));
  boxFilter.setMax(Eigen::Vector4f(bfp.max_x, bfp.max_y, bfp.max_z, 1.0));
  boxFilter.setInputCloud(cloudPtr);
  boxFilter.filter(cloud_filtered);

  // Convert to ROS data type
  sensor_msgs::PointCloud2 output;
  pcl_conversions::fromPCL(cloud_filtered, output);

  // Publish the data
  pub.publish (output);
}

int main (int argc, char** argv)
{
  // Initialize ROS
  ros::init (argc, argv, "pointcloud_filter_a");
  ros::NodeHandle nh;

  // Create a ROS subscriber for the input point cloud
  ros::Subscriber sub = nh.subscribe ("input", 1, cloud_cb);

  // Create a ROS publisher for the output point cloud
  pub = nh.advertise<sensor_msgs::PointCloud2> ("output", 1);

  // Spin
  ros::spin ();
}