#!/bin/bash

RED="\e[31m"
YELLOW="\e[33m"
GREEN="\e[32m"
BOLD="\e[1m"
NL="\n"
RESET="\e[39m\e[0m"

JA_DOCKER_NAME="jaeger-agent:1.17"

if test ${TEMOTO_TRACE_COLLECTOR_IP}; then
  COLLECTOR_IP=${TEMOTO_TRACE_COLLECTOR_IP}
  echo -e $RESET $GREEN $NL"Collector IP set to $COLLECTOR_IP via ENV variable." $RESET
elif test $1; then
  COLLECTOR_IP=$1
  echo -e $RESET $GREEN $NL"Collector IP set to $COLLECTOR_IP via argument." $RESET
else
  echo -e $RESET $RED $NL"Collector IP not specified. Usage: start_tracer_agent.bash [COLLECTOR_IP] or set IP via TEMOTO_TRACE_COLLECTOR_IP env variable." $RESET
  exit
fi

echo -e $RESET$GREEN"Starting $JA_DOCKER_NAME docker container ..." $RESET
docker run \
  --rm \
  -p5775:5775/udp \
  -p6831:6831/udp \
  -p6832:6832/udp \
  -p5778:5778/tcp \
  jaegertracing/$JA_DOCKER_NAME \
  --collector.host-port=$COLLECTOR_IP:14267
