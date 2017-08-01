#!/bin/bash

# Makefile builder script
mkdir build 
cd build

echo "================"
echo "Preparing recipe"
echo "================"
cmake -DCMAKE_CXX_COMPILER=$(which g++) \
      -DCMAKE_CXX_FLAGS="-std=c++11 -Wl" -LAH ..

echo "================"
echo "Building..."
echo "================"
make
