#!/bin/bash

# Makefile builder script
mkdir -p build 
mkdir -p bin
mkdir -p lib
cd build

echo "================"
echo "Preparing recipe"
echo "================"
cmake -DCMAKE_CXX_COMPILER=$(which g++) \
      -DCMAKE_CXX_FLAGS="-std=c++11 -Wall" -LAH ..

echo "================"
echo "Building..."
echo "================"
make
