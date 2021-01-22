This repository contains Robotont/Clearbot specific supplementary materials for reproducing the demos shown in the 
"TeMoto: A Software Framework for Dependable Long-Term Robotic Autonomy with Dynamic Resource Management" paper.

See the [README in the main branch](https://github.com/temoto-telerobotics-demos/temoto_architecture_demos)
for general organization of this repository and for [instructions on how to run the demos](https://github.com/temoto-telerobotics-demos/temoto_architecture_demos/tree/master#run-the-demos).

## Dependencies
The demostrations were set up on Ubuntu 18.04 with ROS Melodic installed.
Additionally few other packages are required to compile some of TeMoto's ROS packages:

``` bash
$ sudo apt install qt5-default
$ sudo apt install ros-melodic-move-base-msgs
$ sudo apt install ros-melodic-moveit-ros-planning-interface
```

Other robot-specific dependencies

``` bash
sudo apt install ros-melodic-usb-cam
```

## Installation
These instructions are specific to this, *robot-clearbot*, branch.

Clone this repository
``` bash
$ git clone --recursive -b robot-clearbot https://github.com/temoto-telerobotics-demos/temoto_architecture_demos
```

Again, this repository is a catkin workspace, so in order to build it:
``` bash
$ cd temoto_architecture_demos
$ catkin build
$ source devel/setup.bash
```