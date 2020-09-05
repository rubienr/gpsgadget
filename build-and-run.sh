#!/bin/bash

ENV="gpsgadget"
TARGET="upload"

#USB=`ls /dev/ttyUSB* | tr ' ' '\n' | sort  | head -1`
USB=`ls /dev/ttyUSB* | tr ' ' '\n' | sort  | tail -1` 

pio run -e $ENV -t $TARGET --upload-port $USB && \
pio device monitor -b 230400 --echo --port $USB

