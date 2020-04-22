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

PG_VERSION=1.2.0
ARCHITECTURE=$(uname -m)

if [ "$ARCHITECTURE" == "aarch64" ]; then
  ARCHITECTURE="arm64"
elif [ "$ARCHITECTURE" == "x86_64" ]; then
  ARCHITECTURE="amd64"
fi

# Pushgateway setup
echo -e $RESET $GREEN $NL"Downloading Pushgateway version $PG_VERSION" $RESET
wget https://github.com/prometheus/pushgateway/releases/download/v$PG_VERSION/pushgateway-$PG_VERSION.linux-$ARCHITECTURE.tar.gz
check_success "Failed to download Pushgateway"

echo -e $RESET $GREEN $NL"Unpacking Pushgateway" $RESET
tar xvzf pushgateway-$PG_VERSION.linux-$ARCHITECTURE.tar.gz
check_success "Failed to unpack Pushgateway"

rm *.gz

echo -e $RESET $GREEN $NL "Done." $RESET