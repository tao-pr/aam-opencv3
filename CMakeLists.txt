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
set(TARGET_ANNOTATOR "aam_annotator")
project(${TARGET_LIB} CXX C)

# Source files
file(GLOB SOURCES src/*.cpp)
set(LIB_SRC src/AAM.cpp)
set(ANNOTATOR_SRC src/Annotator.cpp)

# Targets to build
add_library(${TARGET_LIB} SHARED ${LIB_SRC})
add_executable(${TARGET_ANNOTATOR} ${ANNOTATOR_SRC})
set_target_properties(${TARGET_LIB} PROPERTIES 
  OUTPUT_NAME "aam_opencv3"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set_target_properties(${TARGET_ANNOTATOR} PROPERTIES 
  OUTPUT_NAME "aam_annotator"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")


# Build recipe
set(BUILD_DEPENDENCIES 
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
  libfmt.a)
target_link_libraries( ${TARGET_LIB} ${BUILD_DEPENDENCIES})
target_link_libraries( ${TARGET_ANNOTATOR} ${BUILD_DEPENDENCIES})

message(STATUS "Target project : ${LIB_SRC}")