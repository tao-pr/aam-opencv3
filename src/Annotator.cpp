/**
 * Image annotator
 * ------------------------------
 * Arguments:
 * 
 */

#include "Shape.h"

const string ARGUMENT_KEYS = 
  "{@path           |.       | path to image directories  }";
const string SUPPORTED_EXTS[] = {
  ".png", ".PNG", ".jpg", ".JPG",
  ".jpeg", ".JPEG"
};


static bool supportedFileFormat(const string& path)
{
  for (auto ext : SUPPORTED_EXTS)
  {
    if (path.find(ext) != string::npos)
      return true;
  }
  return false;
}

int main(int argc, char** argv)
{
  CommandLineParser parser(argc, argv, ARGUMENT_KEYS);
  parser.about("Image Annotator Utility");

  string path = parser.get<string>(0);
  cout << GREEN << "Reading images from "
       << YELLOW << path << RESET << endl;
  vector<String> files;
  glob(path, files);

  for (auto file : files)
  {
    auto filepath = file.operator string();
    auto filename = filepath.substr(filepath.find_last_of("/") + 1);
    if (!supportedFileFormat(filename))
      continue;
    cout << CYAN << "Loading image : " << RESET << filename << endl;

    // TAOTODO:
  }

}
