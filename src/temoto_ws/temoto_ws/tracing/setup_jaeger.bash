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

JG_VERSION="0.5.0"

echo -e $RESET $GREEN $BOLD $NL"Downloading jaeger-client-cpp version $JG_VERSION ..." $RESET
git clone --branch v$JG_VERSION https://github.com/jaegertracing/jaeger-client-cpp
check_success "Failed to download jaeger-client-cpp version $JG_VERSION"

echo -e $RESET $GREEN $BOLD $NL"Setting up jaeger-client-cpp version $JG_VERSION ..." $RESET
cd jaeger-client-cpp
mkdir build && cd build
cmake ..
check_success "Failed to invoke CMake on jaeger-client-cpp version $JG_VERSION"

# Use half of the available threads to build jaeger
NR_OF_JOBS=$(echo "$(nproc)/2" | bc)
echo -e $RESET $GREEN $BOLD $NL"Building jaeger-client-cpp version $JG_VERSION with $NR_OF_JOBS parallel jobs ..." $RESET
make -j$NR_OF_JOBS
check_success "Failed to invoke Make on jaeger-client-cpp version $JG_VERSION"