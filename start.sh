#!/bin/sh

export LD_LIBRARY_PATH=$(pwd)/out/lib

./out/bin/generator 1000 -5 20000
./out/bin/server
