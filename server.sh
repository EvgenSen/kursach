#!/bin/sh

export LD_LIBRARY_PATH=$(pwd)/out/lib

./out/bin/generator 100000 -10000 10000
./out/bin/server
