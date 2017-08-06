/**
 * Image annotator
 * ------------------------------
 * Arguments:
 * 
 */

#include "Shape.h"

const string ARGUMENT_KEYS = 
  "{@path |. | path to image directories  }";
const string SUPPORTED_EXTS[] = {
  ".png", ".PNG", ".jpg", ".JPG",
  ".jpeg", ".JPEG"
};

// States
Mat currentImage;
Shape* currentShape = nullptr; 
vector<Point2d> currentAnnotation;
void onAnnotation(int event, int x, int y, int n, void* p);

void renderAnnotation()
{
  Mat canvas = currentImage.clone();
  for (auto v : currentAnnotation)
  {
    circle(canvas, v, 3, Scalar(240,10,0), 1, CV_AA);
  }
  imshow("input", canvas);
  setMouseCallback("input", onAnnotation, nullptr);
}

static bool supportedFileFormat(const string& path)
{
  for (auto ext : SUPPORTED_EXTS)
  {
    if (path.find(ext) != string::npos)
      return true;
  }
  return false;
}

void onAnnotation(int event, int x, int y, int n, void* p)
{
  if (event == EVENT_LBUTTONDOWN)
  {
    // Click to add an annotation
    currentAnnotation.push_back(Point2d(x,y));
    renderAnnotation();
  }
  else if (event == EVENT_RBUTTONDOWN)
  {
    // Right-click to remove the last annotation
    if (!currentAnnotation.empty())
      currentAnnotation.pop_back();
    renderAnnotation();
  }
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

  int n = 1;
  for (auto file : files)
  {
    auto filepath = file.operator string();
    auto filename = filepath.substr(filepath.find_last_of("/") + 1);
    if (!supportedFileFormat(filename))
      continue;
    cout << CYAN << "Loading image : " << RESET << filename << endl;

    currentImage = imread(filepath);
    renderAnnotation();

    waitKey(0);

    // TAOTODO: Save the annotation

    // Clear iterative stages
    currentAnnotation.clear();
    if (currentShape != nullptr)
      delete currentShape;
    ++n;
  }

}
