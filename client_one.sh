#!/bin/sh

ADDR=$1
PORT=$2

export LD_LIBRARY_PATH=$(pwd)/out/lib

if [ -z $PORT ]
then
	echo "Usage: $0 IP_ADDR PORT"
else
	echo "[DBG] ./out/bin/client $PORT $ADDR"
	./out/bin/client $PORT $ADDR
fi
