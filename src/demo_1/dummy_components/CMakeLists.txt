cmake_minimum_required(VERSION 2.8.3)
project(dummy_components)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

## Find catkin macros and libraries
find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES dummy_components
  CATKIN_DEPENDS roscpp std_msgs
#  DEPENDS system_lib
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

# component_string_0
add_executable(component_string_0 src/component_string_0.cpp)
add_dependencies(component_string_0 ${catkin_EXPORTED_TARGETS} ${${PROJECT_NAME}_EXPORTED_TARGETS})
target_link_libraries(component_string_0 ${catkin_LIBRARIES} )

# component_string_1
add_executable(component_string_1 src/component_string_1.cpp)
add_dependencies(component_string_1 ${catkin_EXPORTED_TARGETS} ${${PROJECT_NAME}_EXPORTED_TARGETS})
target_link_libraries(component_string_1 ${catkin_LIBRARIES} )

# component_string_2_a
add_executable(component_string_2_a src/component_string_2_a.cpp)
add_dependencies(component_string_2_a ${catkin_EXPORTED_TARGETS} ${${PROJECT_NAME}_EXPORTED_TARGETS})
target_link_libraries(component_string_2_a ${catkin_LIBRARIES} )

# component_string_2_b
add_executable(component_string_2_b src/component_string_2_b.cpp)
add_dependencies(component_string_2_b ${catkin_EXPORTED_TARGETS} ${${PROJECT_NAME}_EXPORTED_TARGETS})
target_link_libraries(component_string_2_b ${catkin_LIBRARIES} )