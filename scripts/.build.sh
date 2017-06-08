#!/bin/bash

usage() {
    printf "USAGE:
    $0 <TTY> <BAUD_RATE> <BUILD_PATH>
"
    exit 1
}

[ -z $1 ] && usage
[ -z $2 ] && usage
[ -z $3 ] && usage

TTY=$1
BAUD_RATE=$2
BUILD_PATH=$3

echo "$TTY"

rm -rf $BUILD_PATH/{sketch,core,libraries,preproc,../arduino_cache/core/}
mkdir -p $BUILD_PATH/{sketch,core,libraries,preproc,../arduino_cache/core/}

/opt/arduino-1.8.2/arduino-builder -dump-prefs -logger=machine -hardware /opt/arduino-1.8.2/hardware -hardware /root/.arduino15/packages -tools /opt/arduino-1.8.2/tools-builder -tools /opt/arduino-1.8.2/hardware/tools/avr -tools /root/.arduino15/packages -built-in-libraries /opt/arduino-1.8.2/libraries -libraries /root/Arduino/libraries -fqbn=arduino:avr:nano:cpu=atmega328 -ide-version=10802 -build-path $BUILD_PATH -warnings=none -build-cache $BUILD_PATH/../arduino_cache -prefs=build.warn_data_percentage=75 -prefs=runtime.tools.avr-gcc.path=/root/.arduino15/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.4-arduino2 -prefs=runtime.tools.arduinoOTA.path=/root/.arduino15/packages/arduino/tools/arduinoOTA/1.1.1 -prefs=runtime.tools.avrdude.path=/root/.arduino15/packages/arduino/tools/avrdude/6.3.0-arduino9 -verbose /home/boss/Arduino/_7seg/_7seg.ino

/opt/arduino-1.8.2/arduino-builder -compile -logger=machine -hardware /opt/arduino-1.8.2/hardware -hardware /root/.arduino15/packages -tools /opt/arduino-1.8.2/tools-builder -tools /opt/arduino-1.8.2/hardware/tools/avr -tools /root/.arduino15/packages -built-in-libraries /opt/arduino-1.8.2/libraries -libraries /root/Arduino/libraries -fqbn=arduino:avr:nano:cpu=atmega328 -ide-version=10802 -build-path $BUILD_PATH -warnings=none -build-cache $BUILD_PATH/../arduino_cache -prefs=build.warn_data_percentage=75 -prefs=runtime.tools.avr-gcc.path=/root/.arduino15/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.4-arduino2 -prefs=runtime.tools.arduinoOTA.path=/root/.arduino15/packages/arduino/tools/arduinoOTA/1.1.1 -prefs=runtime.tools.avrdude.path=/root/.arduino15/packages/arduino/tools/avrdude/6.3.0-arduino9 -verbose /home/boss/Arduino/_7seg/_7seg.ino

"/root/.arduino15/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.4-arduino2/bin/avr-g++" -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics  -flto -w -x c++ -E -CC -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_NANO -DARDUINO_ARCH_AVR   "-I/root/.arduino15/packages/arduino/hardware/avr/1.6.19/cores/arduino" "-I/root/.arduino15/packages/arduino/hardware/avr/1.6.19/variants/eightanaloginputs" "$BUILD_PATH/sketch/_7seg.ino.cpp" -o "/dev/null"

"/root/.arduino15/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.4-arduino2/bin/avr-g++" -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics  -flto -w -x c++ -E -CC -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_NANO -DARDUINO_ARCH_AVR   "-I/root/.arduino15/packages/arduino/hardware/avr/1.6.19/cores/arduino" "-I/root/.arduino15/packages/arduino/hardware/avr/1.6.19/variants/eightanaloginputs" "$BUILD_PATH/sketch/_7seg.ino.cpp" -o "/tmp/arduino_build_97971/preproc/ctags_target_for_gcc_minus_e.cpp"

"/opt/arduino-1.8.2/tools-builder/ctags/5.8-arduino11/ctags" -u --language-force=c++ -f - --c++-kinds=svpf --fields=KSTtzns --line-directives "$BUILD_PATH/preproc/ctags_target_for_gcc_minus_e.cpp"

"/root/.arduino15/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.4-arduino2/bin/avr-g++" -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_NANO -DARDUINO_ARCH_AVR   "-I/root/.arduino15/packages/arduino/hardware/avr/1.6.19/cores/arduino" "-I/root/.arduino15/packages/arduino/hardware/avr/1.6.19/variants/eightanaloginputs" "$BUILD_PATH/sketch/_7seg.ino.cpp" -o "$BUILD_PATH/sketch/_7seg.ino.cpp.o"

###

"/root/.arduino15/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.4-arduino2/bin/avr-gcc" -w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega328p  -o "$BUILD_PATH/_7seg.ino.elf" "$BUILD_PATH/sketch/_7seg.ino.cpp.o" "$BUILD_PATH/../arduino_cache/core/core_arduino_avr_nano_cpu_atmega328_67cb05eecae7be3f830899c6612c30f7.a" "-L/$BUILD_PATH" -lm

"/root/.arduino15/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.4-arduino2/bin/avr-objcopy" -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0  "$BUILD_PATH/_7seg.ino.elf" "$BUILD_PATH/_7seg.ino.eep"

"/root/.arduino15/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.4-arduino2/bin/avr-objcopy" -O ihex -R .eeprom  "$BUILD_PATH/_7seg.ino.elf" "$BUILD_PATH/_7seg.ino.hex"


