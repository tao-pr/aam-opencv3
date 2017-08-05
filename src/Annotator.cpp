/**
 * Image annotator
 * ------------------------------
 * Arguments:
 * 
 */

#include "Shape.h"

const string ARGUMENT_KEYS = 
  "{@path           |.       | path to image directories  }";

int main(int argc, char** argv)
{
  CommandLineParser parser(argc, argv, ARGUMENT_KEYS);
  parser.about("Image Annotator Utility");

  string path = parser.get<string>(0);

  cout << "Reading images from " << path << endl;
  vector<String> files;
  glob(path, files);

  for (auto file : files)
  {
    auto filename = file.operator string();
  }

}
