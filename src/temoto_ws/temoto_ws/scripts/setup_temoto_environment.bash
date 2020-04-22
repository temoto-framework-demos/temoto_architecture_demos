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

# Get the name of the temoto workspace
if test $1; then
  TEMOTO_WS_NAME=$1
else
  echo -e $RESET $RED $NL"TeMoto workspace packgage not specified." $RESET
  exit
fi

# Get the unique name for this temoto instance
if test $2; then
  TEMOTO_INSTANCE_NAME=$2
else
  echo -e $RESET $RED $NL"TeMoto instance name not specified." $RESET
  exit
fi

# Get the ip address of ROS master
if test $3; then
  ROS_MASTER_IP=$3
else
  echo -e $RESET $RED $NL"ROS MASTER IP not specified." $RESET
  exit
fi

# Get the ip address of ROS master
if test $4; then
  LOCAL_IP=$4
else
  echo -e $RESET $RED $NL"LOCAL IP not specified." $RESET
  exit
fi

# Get the ip address of ROS master
if test $5; then
  TRACE_COLLECTOR_IP=$5
else
  echo -e $RESET $RED $NL"TRACE COLLECTOR IP not specified." $RESET
  exit
fi  

TEMOTO_WS_PATH=$(rospack find $TEMOTO_WS_NAME)
check_success "Failed to find package $TEMOTO_WS_NAME"

if [[ $ROS_MASTER_IP != "local" ]]; then
  export ROS_MASTER_URI=http://$ROS_MASTER_IP:11311
  echo -e $RESET$GREEN$BOLD"ROS_MASTER_URI$RESET$GREEN set to $ROS_MASTER_IP" $RESET
fi

if [[ $LOCAL_IP != "local" ]]; then
  export ROS_IP=$LOCAL_IP
  echo -e $RESET$GREEN$BOLD"ROS_IP$RESET$GREEN set to $LOCAL_IP" $RESET
fi

if [[ $TRACE_COLLECTOR_IP != "local" ]]; then
  export TEMOTO_TRACE_COLLECTOR_IP=$TRACE_COLLECTOR_IP
  echo -e $RESET$GREEN$BOLD"TEMOTO_TRACE_COLLECTOR_IP$RESET$GREEN set to $TRACE_COLLECTOR_IP" $RESET
fi

export TEMOTO_NAMESPACE=$TEMOTO_INSTANCE_NAME
echo -e $RESET$GREEN$BOLD"TEMOTO_NAMESPACE$RESET$GREEN set to $TEMOTO_NAMESPACE" $RESET

alias start_tracer_agent="cur_path=$(pwd) && cd $TEMOTO_WS_PATH/tracing/ && bash start_tracer_agent.bash && cd $cur_path"
echo -e $RESET$GREEN$BOLD"set alias$RESET$GREEN start_tracer_agent" $RESET

alias start_sending_metrics="cur_path=$(pwd) && cd $TEMOTO_WS_PATH/monitoring/ && bash start_sending_metrics.bash && cd $cur_path"
echo -e $RESET$GREEN$BOLD"set alias$RESET$GREEN start_sending_metrics" $RESET
