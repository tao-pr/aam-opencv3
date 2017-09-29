/**
 * Master.h is supposed to be included by all header files 
 * inside the project
 */

#ifndef MASTER_H
#define MASTER_H

#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <assert.h>
#include <fmt/format.h>
#include <stdio.h>
#include <dirent.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

const std::string RED("\033[0;31m");
const std::string GREEN("\033[1;32m");
const std::string YELLOW("\033[1;33m");
const std::string CYAN("\033[0;36m");
const std::string MAGENTA("\033[0;35m");
const std::string RESET("\033[0m");

#endif