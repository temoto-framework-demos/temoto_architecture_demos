#!/bin/bash

PG_VERSION=1.2.0
ARCHITECTURE=$(uname -m)

if [ "$ARCHITECTURE" == "aarch64" ]; then
  ARCHITECTURE="arm64"
elif [ "$ARCHITECTURE" == "x86_64" ]; then
  ARCHITECTURE="amd64"
fi

${TEMOTO_WORKSPACE_PATH}/monitoring/metrics_parser &
${TEMOTO_WORKSPACE_PATH}/monitoring/pushgateway-$PG_VERSION.linux-$ARCHITECTURE/pushgateway &&
kill $!
