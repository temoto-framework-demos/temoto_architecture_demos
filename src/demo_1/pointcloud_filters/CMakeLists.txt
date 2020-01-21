cmake_minimum_required(VERSION 2.8.3)
project(pointcloud_filters)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  pcl_conversions
  pcl_ros
  roscpp
  sensor_msgs
)


catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES pointcloud_filter_a
#  CATKIN_DEPENDS pcl_conversions pcl_ros roscpp sensor_msgs
#  DEPENDS system_lib
)

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

add_executable(pointcloud_filter_a src/pointcloud_filter_a.cpp)
target_link_libraries(pointcloud_filter_a ${catkin_LIBRARIES})

add_executable(pointcloud_filter_b src/pointcloud_filter_b.cpp)
target_link_libraries(pointcloud_filter_b ${catkin_LIBRARIES})