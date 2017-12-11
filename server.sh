#!/bin/sh

export LD_LIBRARY_PATH=$(pwd)/out/lib

./out/bin/generator 100000 -1000000 1000000
./out/bin/server
