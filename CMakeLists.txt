cmake_minimum_required(VERSION 3.8)

# Fmt library, assumes installation via Homebrew
set(FMT_LIB_DIR /usr/local/Cellar/fmt/4.0.0)

# Dependencies
include_directories(/usr/local/include)
include_directories("$ENV{OPENCV_DIR}/include")
include_directories("${FMT_LIB_DIR}/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/headers")
link_directories(/usr/local/lib)
link_directories("$ENV{OPENCV_DIR}/lib")
link_directories("${FMT_LIB_DIR}/lib")

set(TARGET_LIB "aam_opencv3")
project(${TARGET_LIB} CXX C)

# Source files
file(GLOB SOURCES src/*.cpp)
set(LIB_SRC src/AAM.cpp)

# Targets to build
add_library(${TARGET_LIB} SHARED ${LIB_SRC})
set_target_properties(${TARGET_LIB} PROPERTIES 
  OUTPUT_NAME "aam_opencv3"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")


target_link_libraries( ${TARGET_LIB}
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
  libfmt.a
)

message(STATUS "Target project : ${LIB_SRC}")