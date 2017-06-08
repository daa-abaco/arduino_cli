#!/bin/bash
PROJ="_7seg" &&\
  [ -n "$1" ] && PROJ="$1"

export PROJ="$PROJ" 

`dirname $0`/build.sh &&\
`dirname $0`/upload.sh &&\
`dirname $0`/connect.sh


