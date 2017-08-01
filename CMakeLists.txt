cmake_minimum_required(VERSION 3.8)

# Dependencies
include_directories(/usr/local/include)
include_directories("$ENV{OPENCV_DIR}/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/headers")
link_directories(/usr/local/lib)
link_directories("$ENV{OPENCV_DIR}/lib")

set(TARGET "aam_opencv3")
project(${TARGET} CXX C)

# Source files
file(GLOB SOURCES src/*.cpp)
set(LIB_SRC src/Appearance.cpp)

# Targets
add_library(${TARGET} SHARED ${LIB_SRC})
set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME "aam_opencv3")


target_link_libraries( ${TARGET}
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

message(STATUS "Target project : ${LIB_SRC}")