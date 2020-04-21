#!/bin/bash

RED="\e[31m"
YELLOW="\e[33m"
GREEN="\e[32m"
BOLD="\e[1m"
NL="\n"
RESET="\e[39m\e[0m"

# Checks if previously executed process finished successfully or not
# Usage: check_success() <"Error message">
check_success() {
  if [[ $? != 0 ]]; then
    echo -e $RED$BOLD$1". Exiting"$RESET
    exit
  fi
}

echo -e $GREEN"Setting ROS_IP=$1 and ROS_MASTER_URI=$http://$2:11311"$RESET

export ROS_IP=$1
export ROS_MASTER_URI=http://$2:11311
