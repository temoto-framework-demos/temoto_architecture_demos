This repository contains supplementary materials for reproducing the demos shown in the *"TeMoto: A Software Framework for Dependable Long-Term Robotic Autonomy with Dynamic Resource Management"* paper.

The platform-specific configuration files and dependencies are under the according branch, which include:
 * [**Clearpath Jackal**](https://github.com/temoto-telerobotics-demos/temoto_architecture_demos/tree/robot-jackal)
 * [**Robotont**](https://github.com/temoto-telerobotics-demos/temoto_architecture_demos/tree/robot-clearbot)
 * [**Ufactory xArm7**](https://github.com/temoto-telerobotics-demos/temoto_architecture_demos/tree/robot-xarm7)
 * [**Operator Control Station**](https://github.com/temoto-telerobotics-demos/temoto_architecture_demos/tree/operator)

There are three demos in total:
 * **Demo 1** - *"Fault Tolerant Sensor Redundancy"* - exemplifies how the ability to dynamically load resources can be utilized as a backbone structure of a redundant and fault-tolerant robotic system
 * **Demo 2** - *"Redundant Multi-Robot Escort via Resource Synchronization"* - exemplifies the use of resource synchronization features of TeMoto.
 * **Demo 3** - *"Mission Adaptiveness During Long-Term Deployment"* - showcases how a deployed robot can be adapted for a new task by describing the missions via UMRF graph.

## General Requirements
The demostrations were set up on Ubuntu 18.04 with ROS Melodic installed.
Additionally Qt5 is required to compile some of TeMoto's ROS packages:

``` bash
$ sudo apt install qt5-default
``` 

## Layout of the Repository
This repository (and each platform-specific branch) is a ROS1 catkin workspace with the following structure

**src/**
* **temoto/** - contains TeMoto-related base subsystems
* **robot_setup/** - *(branch specific content)* contains robot-specific packages
* **temoto_ws/** - *(branch specific content)* contains robot related TeMoto configuration files
   * **actions/** - contains TeMoto actions. Each action is organized as a ROS1 package
   * **temoto_ws/**
     * **config/** - contains *components.yaml*, *pipes.yaml*, *robot_description.yaml* files 
     * **launch/** - launch files for launching TeMoto
     * **monitoring/** - scripts and setup files for CPU and power monitoring
     * **tracing/** - scripts and setup files for [OpenTracing](https://opentracing.io/) based distributed tracing
     * **scripts/** - convenience scripts

*scripts*, *monitoring* and *tracing* folders do not contain anything that is required for just running the demos.
However if monitoring and/or tracing functionalities are desired, then these folders contain the necessary setup files.

## Setting Up the Repository
Follow the platform-specific instructions for detailed installation process

Clone this repository
``` bash
$ git clone --recursive https://github.com/temoto-telerobotics-demos/temoto_architecture_demos
```

Again, this repository is a catkin workspace, so in order to build it:
``` bash
$ cd temoto_architecture_demos
$ catkin build
$ source devel/setup.bash
```

## Run the Demos
Conceptually each platform contains its own TeMoto configuration and can operate individually or as a multi robot system.
This means that TeMoto is deployed on every platform, not on a single central platform. The robots communicate via
wireless local network.

In all demos, the OCS (operator) was configured as the ROS master. Follow [this tutorial](https://github.com/ut-ims-robotics/tutorials/wiki/Running-ROS-over-multiple-computers) for configuring the ROS master. For convenience I suggest using SSH to operate with the robots.

### Demo 1
The goal of this demo is to exemplify how the ability to dynamically load resources can be utilized as a backbone structure of a redundant and fault-tolerant robotic system. The underlying scenario depicts a remote inspection mission where a human operator controls a robot based on the visual feedback provided by the robot’s sensors. The essential part of the mission is sustaining the operator’s situational awareness, which is challenged by incidents that compromise the robot’s sensors. As a countermeasure for continuing the mission, each resource failure is addressed by utilizing an alternative sensor onboard the robot.

#### Running the Demo
Launch TeMoto on each platform individually:
``` bash
# on OCS
$ roslaunch temoto_ws temoto.launch temoto_namespace:=ocs

# on Jackal
$ roslaunch temoto_ws temoto.launch temoto_namespace:=jackal
```

On OCS: Invoke the action on the OCS, which initializes Jackal's hardware communication, required sensors, displays the data on OCS's screen via RViz,
and embeds the fault tolerance mechanism that initializes an alternative sensor in case of a sensor failure.
``` bash
$ roslaunch ta_redundant_teleop_viz action_test_separate.launch wake_word:=ocs
```

Now the platforms are set. The rest of the scenario is about manually introducing faults. One can either remove the data
cable from the sensors, as shown in demo videos, or the associated sensor driver can be forced to shut down via:
``` bash
kill <PROCESS_ID>
```

### Demo 2
The goal of this demo is to exemplify the use of resource synchronization features of TeMoto. Similarly to Demo 1, the underlying scenario depicts a remote inspection mission where a human operator controls a robot based on the visual feedback provided by the robot’s camera. As a part of the demonstration, the visual sensing capabilities of the teleoperated robot are severed but automatically compensated by another robot in the vicinity, which helps to escort the compromised robot back to the operator’s reach.

#### Running the Demo
Launch TeMoto on each platform individually:
``` bash
# on OCS
$ roslaunch temoto_ws temoto.launch temoto_namespace:=ocs

# on Jackal
$ roslaunch temoto_ws temoto.launch temoto_namespace:=jackal

# on Robotont
$ roslaunch temoto_ws temoto.launch temoto_namespace:=robotont
```

On OCS: Invoke the action on the OCS, which initializes Jackal's hardware communication, required sensors, displays the data on OCS's screen via RViz, 
and embeds the fault tolerance mechanism that makes Robotont share its camera stream and track Jackal.
``` bash
$ roslaunch ta_remote_sensor_control action_test_separate.launch wake_word:=ocs
```

Now the platforms are set. The rest of the scenario is about manually introducing a fault in Jackal's camera. One can either remove the data
cable from it, as shown in demo videos, or the associated sensor driver (usb_cam) can be forced to shut down via:
``` bash
kill <PROCESS_ID>
```

### Demo 3
This demo showcases how a deployed robot can be adapted for a new task by describing the missions via UMRF graph. The scenario depicts a mobile robot autonomously sweeping an area as its “day job” until the robot is retasked to deliver goods, handed over by another robot. After successfully completing the delivery, the robot  continues with the surveillance mission.

#### Running the Demo
Launch TeMoto on each platform individually:
``` bash
# on OCS
$ roslaunch temoto_ws temoto.launch temoto_namespace:=ocs

# on Jackal
$ roslaunch temoto_ws temoto.launch temoto_namespace:=jackal

# on xArm7
$ roslaunch temoto_ws temoto.launch temoto_namespace:=xarm7
```

On OCS: Invoke the UMRF graph that describes the "surveillance mission", whick makes Jackal navigate through 3 locations repeatedly
``` bash
rosrun temoto_action_engine parser_node <PATH_TO_>/temoto_architecture_demos/src/temoto_ws/umrf_graphs/demo3_surveillance_mission.umrfg.json ocs
```

Stop the "surveillance mission" whenever needed
``` bash
rostopic pub /stop_umrf_graph_topic temoto_action_engine/StopUmrfJsonGraph "graph_name: 'demo3_surveillance_mission'
targets:
- 'ocs'"
```

On OCS: Invoke the UMRF graph that describes the "delivery mission", which makes Jackal navigate next to xArm7, commands
xArm7 to drop two objects and finally commands Jackal to navigate to the delivery location 
``` bash
rosrun temoto_action_engine parser_node <PATH_TO_>/temoto_architecture_demos/src/temoto_ws/umrf_graphs/demo3_transportation_mission.umrfg.json ocs
```