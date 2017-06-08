#!/bin/bash
TTY="/dev/ttyUSB0"
BAUD_RATE=9600

[ -n "$1" ] && TTY=$1
[ -n "$2" ] && BAUD_RATE=$2

echo "Starting terminal..."
picocom $TTY -b $BAUD_RATE
