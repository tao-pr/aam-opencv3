#!/bin/bash

export OPENCV_DIR=/usr/local/Cellar/opencv/4.1.0_2/
export OPENCV_INCLUDE_DIR=$OPENCV_DIR/include/opencv4/
export OPENCV_LIB_DIR=$OPENCV_DIR/lib/

# Makefile builder script
mkdir -p build 
mkdir -p bin
mkdir -p lib
cd build

echo "================"
echo "Preparing recipe"
echo "================"
cmake -DCMAKE_CXX_COMPILER=$(which g++) \
      -DCMAKE_CXX_FLAGS="-std=c++11 -Wall -g -O1" -LAH ..

echo "================"
echo "Building..."
echo "================"
make
