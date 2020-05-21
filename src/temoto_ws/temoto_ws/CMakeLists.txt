cmake_minimum_required(VERSION 2.8.3)
project(temoto_ws)

# use c++11 standard
add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  temoto
  temoto_action_assistant
  robotont_driver
  robotont_description
  ar_track_alvar
  geometry_msgs
  usb_cam
)

catkin_package()

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

catkin_install_python(PROGRAMS scripts/ar_tag_follower.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
