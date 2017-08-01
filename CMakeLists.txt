cmake_minimum_required(VERSION 3.8)

# Dependencies
include_directories(/usr/local/include)
include_directories("$ENV{OPENCV_DIR}/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/headers")
link_directories(/usr/local/lib)
link_directories("$ENV{OPENCV_DIR}/lib")

set(CMAKE_BUILD_TYPE Release)
project(aam-opencv3 CXX C)

# Source files
file(GLOB SOURCES src/*.cpp)

# Libraries
set(LIB_SRC src/Appearance.cpp src/Shape.cpp)

target_link_libraries(
  opencv_core 
  opencv_imgproc 
  opencv_video
  opencv_videoio
  opencv_features2d 
  opencv_ml 
  opencv_highgui
  opencv_imgcodecs 
  opencv_objdetect
  opencv_xfeatures2d
)

# Targets
add_library(aam-opencv3 SHARED ${LIB_SRC})