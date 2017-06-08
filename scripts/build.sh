#!/bin/bash
TTY="/dev/ttyUSB0"
BAUD_RATE=9600
BUILD_PATH="/home/boss/Arduino/$PROJ/arduino_build"

[ -n "$1" ] && TTY=$1
[ -n "$2" ] && BAUD_RATE=$2
[ -n "$3" ] && BUILD_PATH=$3

`dirname $0`/.build.sh $TTY $BAUD_RATE $BUILD_PATH
