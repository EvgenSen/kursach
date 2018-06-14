#!/bin/sh

PORT=$1

export LD_LIBRARY_PATH=$(pwd)/out/lib

if [ -z $PORT ]
then
	echo "Usage: $0 PORT"
else
	echo "[DBG] ./out/bin/client $PORT"
	./out/bin/client $PORT
fi
