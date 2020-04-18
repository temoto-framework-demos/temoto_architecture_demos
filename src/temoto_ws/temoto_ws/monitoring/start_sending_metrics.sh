#!/bin/bash

./metrics_parser &
./pushgateway-0.8.0.linux-amd64/pushgateway &&
kill $!