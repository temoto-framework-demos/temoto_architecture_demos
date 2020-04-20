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

# Pushgateway setup
echo -e $RESET $GREEN $NL"Downloading Pushgateway version 0.8.0" $RESET
wget https://github.com/prometheus/pushgateway/releases/download/v0.8.0/pushgateway-0.8.0.linux-amd64.tar.gz
check_success "Failed to download Pushgateway"

echo -e $RESET $GREEN $NL"Unpacking Pushgateway" $RESET
tar xvzf pushgateway-0.8.0.linux-amd64.tar.gz
check_success "Failed to unpack Pushgateway"

rm *.gz

echo -e $RESET $GREEN $NL "Done." $RESET