#!/bin/bash
TTY="/dev/ttyUSB0"
BAUD_RATE=9600
BUILD_PATH="/home/boss/Arduino/$PROJ/arduino_build"

[ -n "$1" ] && TTY=$1
[ -n "$2" ] && BAUD_RATE=$2
[ -n "$3" ] && BUILD_PATH=$3

#`dirname $0`/build.sh $TTY $BAUD_RATE $BUILD_PATH | tee build.log

echo "Uploading elf file..."

/root/.arduino15/packages/arduino/tools/avrdude/6.3.0-arduino9/bin/avrdude -C/root/.arduino15/packages/arduino/tools/avrdude/6.3.0-arduino9/etc/avrdude.conf -v -patmega328p -carduino -P$TTY -b57600 -D -Uflash:w:$BUILD_PATH/../arduino_build/$PROJ.ino.hex:i 
