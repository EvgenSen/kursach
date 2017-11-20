#!/bin/sh

export LD_LIBRARY_PATH=$(pwd)/out/lib

./out/bin/generator 500 -50 150
./out/bin/server
