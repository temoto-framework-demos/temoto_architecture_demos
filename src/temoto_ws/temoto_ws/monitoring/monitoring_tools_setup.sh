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

# Prometheus setup
echo -e $RESET $GREEN $NL"Downloading Prometheus version 2.9.2" $RESET
wget https://github.com/prometheus/prometheus/releases/download/v2.9.2/prometheus-2.9.2.linux-amd64.tar.gz
check_success "Failed to download Prometheus"

echo -e $RESET $GREEN $NL"Unpacking Prometheus" $RESET
tar xvzf prometheus-2.9.2.linux-amd64.tar.gz
check_success "Failed to unpack Prometheus"

cp prometheus.yml prometheus-2.9.2.linux-amd64/prometheus.yml
check_success "Failed set up the prometheus.yml file"

# Grafana setup
echo -e $RESET $GREEN $NL"Downloading Grafana 6.7.2" $RESET
wget https://dl.grafana.com/oss/release/grafana-6.7.2.linux-amd64.tar.gz
check_success "Failed to download Grafana"

echo -e $RESET $GREEN $NL"Unpacking Grafana" $RESET
tar -zxvf grafana-6.7.2.linux-amd64.tar.gz
check_success "Failed to unpack Grafana"

rm *.gz

echo -e $RESET $GREEN $NL"Follow the Grafana setup guide in: https://devconnected.com/monitoring-linux-processes-using-prometheus-and-grafana/" $RESET