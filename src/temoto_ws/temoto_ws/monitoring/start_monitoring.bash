#!/bin/bash

./prometheus-2.9.2.linux-amd64/prometheus --config.file ./prometheus-2.9.2.linux-amd64/prometheus.yml &
cd grafana-6.7.2 &&
./bin/grafana-server &&
kill $!