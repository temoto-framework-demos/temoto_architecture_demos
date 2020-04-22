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
COPY_CONFIG=$1

if [[ ! -d jaeger-client-cpp ]]; then
  echo -e $RESET $GREEN $BOLD $NL"Downloading jaeger-client-cpp version $JG_VERSION ..." $RESET
  git clone --branch v$JG_VERSION https://github.com/jaegertracing/jaeger-client-cpp
  check_success "Failed to download jaeger-client-cpp version $JG_VERSION"
fi

echo -e $RESET $GREEN $BOLD $NL"Setting up jaeger-client-cpp version $JG_VERSION ..." $RESET
current_dir=$(pwd)
if [[ ! -d jaeger-client-cpp/build ]]; then
  cd jaeger-client-cpp
  mkdir build && cd build
  cmake ..
  check_success "Failed to invoke CMake on jaeger-client-cpp version $JG_VERSION"
else
  cd jaeger-client-cpp/build
  cmake ..
  check_success "Failed to invoke CMake on jaeger-client-cpp version $JG_VERSION"
fi

# Use half of the available threads to build jaeger
NR_OF_JOBS=$(echo "$(nproc)/2" | bc)
echo -e $RESET $GREEN $BOLD $NL"Building jaeger-client-cpp version $JG_VERSION with $NR_OF_JOBS parallel jobs ..." $RESET
make -j$NR_OF_JOBS
check_success "Failed to invoke Make on jaeger-client-cpp version $JG_VERSION"

cd $current_dir

if [[ $COPY_CONFIG == "--copy-config" ]]; then
  echo -e $RESET $GREEN $BOLD $NL"Setting up the tracer_config.yaml in temoto_core ..." $RESET
  temoto_core_path="$(rospack find temoto_core)"
  check_success "Failed to find the location of temoto_core"

  tracer_config_path=$temoto_core_path/config/tracer_config.yaml
  cp tracer_config.yaml $tracer_config_path
  check_success "Failed to copy the tracer_config.yaml to $tracer_config_path"

  jaeger_lib_path="$(pwd)/jaeger-client-cpp/build/libjaegertracing.so"
  jaeger_lib_path_esc=$(echo "$jaeger_lib_path" | sed -e 's/[]\/$*.^[]/\\&/g')
  sed -i "s/TODO_PATH/$jaeger_lib_path_esc/g" $tracer_config_path
  check_success "Failed to set library_path in $tracer_config_path to $jaeger_lib_path"
fi

echo -e $RESET $GREEN $BOLD $NL"Done" $RESET
