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

trap "exit" INT TERM ERR
trap "kill 0" EXIT

bash ../monitoring/start_sending_metrics.bash &
bash ../tracing/start_tracer_agent.bash &
sleep 2 &&
roslaunch temoto_ws temoto.launch temoto_namespace:=$TEMOTO_NAMESPACE &

wait

