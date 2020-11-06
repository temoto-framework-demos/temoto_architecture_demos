cmake_minimum_required(VERSION 2.8.3)
project(temoto_ws)

# use c++11 standard
add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  temoto
  temoto_action_assistant
  geometry_msgs
)

catkin_package()

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_executable(cmd_vel_stabilizer
  src/cmd_vel_stabilizer.cpp
)

add_dependencies(cmd_vel_stabilizer
  ${catkin_EXPORTED_TARGETS}
  ${${PROJECT_NAME}_EXPORTED_TARGETS}
)

target_link_libraries(cmd_vel_stabilizer
  ${catkin_LIBRARIES}
)
