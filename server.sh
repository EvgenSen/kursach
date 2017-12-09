#!/bin/sh

export LD_LIBRARY_PATH=$(pwd)/out/lib

./out/bin/generator 120000 -10000 10000
./out/bin/server
