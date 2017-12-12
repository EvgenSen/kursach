#!/bin/sh

export LD_LIBRARY_PATH=$(pwd)/out/lib

# При 5к элементах стабильно работает 
./out/bin/generator 5000 -10000 10000
./out/bin/server
