cmake_minimum_required(VERSION 3.8)

# Fmt library, assumes installation via Homebrew
set(FMT_LIB_DIR /usr/local/Cellar/fmt/5.3.0)

# Dependencies
include_directories(/usr/local/include)
include_directories("$ENV{OPENCV_DIR}/include")
include_directories("${FMT_LIB_DIR}/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/headers")
link_directories(/usr/local/lib)
link_directories("$ENV{OPENCV_DIR}/lib")
link_directories("${FMT_LIB_DIR}/lib")

project("aam-opencv3" CXX C)

set(TARGET_LIB "aam-lib")
set(TARGET_ANNOTATOR "aam_annotator")
set(TARGET_TEST "aam-test")

# Source files
# file(GLOB SOURCES src/*.cpp)
file(GLOB SOURCES 
  "${CMAKE_CURRENT_SOURCE_DIR}/headers/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lib/*.cpp"
)
set(ANNOTATOR_SRC src/exe/Annotator.cpp)
set(TEST_SRC src/test/Test.cpp)

# Third-party dependencies
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


# Targets to build
add_library(${TARGET_LIB} SHARED ${SOURCES})
add_executable(${TARGET_ANNOTATOR} ${ANNOTATOR_SRC})
add_executable(${TARGET_TEST} ${TEST_SRC})
set_target_properties(${TARGET_LIB} PROPERTIES 
  OUTPUT_NAME "aam_lib"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set_target_properties(${TARGET_ANNOTATOR} PROPERTIES 
  OUTPUT_NAME "aam_annotator"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set_target_properties(${TARGET_TEST} PROPERTIES
  OUTPUT_NAME "aam_test"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

# Build recipe
target_link_libraries( ${TARGET_LIB} ${BUILD_DEPENDENCIES})
target_link_libraries( ${TARGET_ANNOTATOR} ${BUILD_DEPENDENCIES} ${TARGET_LIB})
target_link_libraries( ${TARGET_TEST} ${BUILD_DEPENDENCIES} ${TARGET_LIB})

message(STATUS "OUTPUT binary objects  : ${CMAKE_BINARY_DIR}")
message(STATUS "Target project : ${LIB_SRC}")
message(STATUS "Target project : ${TEST_SRC}")