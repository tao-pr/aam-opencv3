#include "Test.h"

void testMeshShape(char** argv)
{
  const int NUM_VERTICES = 32;
  cout << GREEN << "Initialising mesh shape ..." << RESET << endl;
  MeshShape mesh = initialMesh(NUM_VERTICES);
  int numTriangles = mesh.numTriangles();

  cout << NUM_VERTICES << " vertices, " << numTriangles << " triangles" << endl;

  auto ioMesh = IO::WindowIO("mesh");
  mesh.render(&ioMesh, Mat::zeros(CANVAS_SIZE, CANVAS_SIZE, CV_8UC3));
  waitKey(0);
}

void testShape(char** argv)
{
  const int TRAIN_SET_SIZE = 16;
  const int SHAPE_SIZE     = 7;
  const double TOL         = 1e-3;
  const int MAX_ALIGN_ITER = 10;
  unique_ptr<ShapeCollection> trainset = initialShapeCollection(TRAIN_SET_SIZE, SHAPE_SIZE);

  // Render each shape onto the same plane
  cout << CYAN << "[#] Shape scaling & translation " << RESET << endl;
  auto io = IO::WindowIO("shapes");
  trainset->renderShapeVariation(&io, Size(CANVAS_SIZE, CANVAS_SIZE));

  // Remove translations & scalings
  auto ioNrm = IO::WindowIO("scaling + translated", Point(CANVAS_SIZE+10, 0));
  trainset->normaliseScalingTranslation();
  trainset->renderShapeVariation(
    &ioNrm, 
    Size(CANVAS_SIZE, CANVAS_SIZE),
    Aux::square(CANVAS_HALFSIZE), // scale the distance
    Point2d(CANVAS_HALFSIZE, CANVAS_HALFSIZE));
  moveWindow("scaling + translated", CANVAS_SIZE+10, 0);

  // cout << GREEN << "[Shapes without translation and scaling]" << RESET << endl;
  // for (auto s : scaledSet.getItems())
  // {
  //   cout << YELLOW << "***********************" << RESET << endl;
  //   cout << s.mat << endl;
  // }

  // Remove rotations
  cout << CYAN << "[#] Shape rotation " << RESET << endl;
  auto ioPc = IO::WindowIO("rotated", Point(CANVAS_SIZE+20, 0));
  trainset->renderShapeVariation(
    &ioPc,
    Size(CANVAS_SIZE, CANVAS_SIZE),
    Aux::square(CANVAS_HALFSIZE), // scale the distance
    Point2d(CANVAS_HALFSIZE, CANVAS_HALFSIZE));
  moveWindow("rotated", (CANVAS_SIZE+10)*2, 0);

  // cout << GREEN << "[Shapes without rotation]" << RESET << endl;
  // for (auto s : rotatedSet.getItems())
  // {
  //   cout << YELLOW << "***********************" << RESET << endl;
  //   cout << s.mat << endl;
  // }

  // Find the mean shape by Procrustes Analysis
  // and align all shapes to that mean
  // NOTE: We don't use the rotated set above
  cout << CYAN << "[#] Shape procrustes mean " << RESET << endl;
  auto alignedSet = trainset->clone();
  auto meanModel = alignedSet->procrustesMean(TOL, MAX_ALIGN_ITER);
  auto ioAl   = IO::WindowIO("aligned", Point(0, CANVAS_SIZE+25));
  auto ioMean = IO::WindowIO("mean", Point(CANVAS_SIZE+10, CANVAS_SIZE+25));
  
  // Re-scale and re-centre the mean shape before rendering
  Shape* meanShape = dynamic_cast<Shape*>(meanModel);
  meanShape->recentreAndScale(Point2d(CANVAS_HALFSIZE, CANVAS_HALFSIZE), Aux::square(CANVAS_HALFSIZE));
  meanShape->render(&ioMean, Mat(CANVAS_SIZE, CANVAS_SIZE, CV_8UC3, Scalar(80,20,5)));
  
  auto alignedShapeSet = dynamic_cast<ShapeCollection*>(alignedSet.get());
  alignedShapeSet->renderShapeVariation(
    &ioAl,
    Size(CANVAS_SIZE, CANVAS_SIZE),
    Aux::square(CANVAS_HALFSIZE), // scale the distance
    Point2d(CANVAS_HALFSIZE, CANVAS_HALFSIZE));
  moveWindow("aligned", (CANVAS_SIZE+10)*3, 0);
  moveWindow("mean", 0, CANVAS_SIZE+50);

  // Calculate covariance
  cout << CYAN << "[#] Shape collection covariance " << RESET << endl;
  auto cov = alignedSet->covariance(meanShape);
  Mat covResized;
  resize(cov, covResized, Size(CANVAS_SIZE, CANVAS_SIZE), INTER_LINEAR);
  normalize(covResized, covResized, 255, 0, NORM_L2);
  imshow("covariance", covResized);
  moveWindow("covariance", (CANVAS_SIZE+10),(CANVAS_SIZE+50));

  // Calculate PCA shape
  cout << CYAN << "[#] Shape PCA " << RESET << endl;
  auto eigenShape = alignedSet->clone()->pca(meanShape);

  // Encode shapes with PCA and measure the estimation errors
  int i = 0;
  cout << GREEN << "[PCA-parameterised shapes]" << RESET << endl;
  for (auto model : alignedSet->getItems())
  {
    const Shape* shape = dynamic_cast<const Shape*>(model);
    cout << CYAN << "... Encoding shape # " << RESET << i << endl;
    auto param = eigenShape.encode(shape);
    cout << "... Decoding shape # " << i << endl;
    auto encodedShape = eigenShape.toShape(param);
    double error = shape->procrustesDistance(&encodedShape);
    cout << "... Estimation error : " << error << endl;
    i++;
  }
}

void testTexture(char** argv)
{
  cout << GREEN << "Loading sample image" << RESET << endl;
  Mat original = imread("media/numbers.jpg", CV_LOAD_IMAGE_COLOR);
  Rect crop(20, 20, 160, 160);
  Mat plane = original(crop);

  // Source triangle coordinates
  vector<Point2d> A0
  {
    Point2d(0, 0),
    Point2d(plane.cols-1, 0),
    Point2d(0, plane.rows-1),
  };
  vector<Point2d> A1
  {
    Point2d(plane.cols-1, 0),
    Point2d(plane.cols-1, plane.rows-1),
    Point2d(0, plane.rows-1)
  };
  auto source = {A0, A1};

  // Destination triangle coordinates
  const vector<Point2d> B0
  {
    Point2d(25, 25),
    Point2d(200, 75),
    Point2d(140, 350)
  };
  const vector<Point2d> B1
  {
    Point2d(200, 75),
    Point2d(250, 350),
    Point2d(140, 350)
  };
  vector<vector<Point2d>> dest = {B0, B1};

  // Source texture to warp
  Mat p = plane;
  IO::WindowIO ioT("source");
  vector<Texture> textures;
  for (auto t:source)
  {
    Texture tx(Triangle(t), &plane);
    p = tx.render(&ioT, p, true, true);
    textures.push_back(tx);
  }
  
  // Re-align to the new triangles & render
  Size sizeAligned(500, 500);
  Mat canvas = Mat(sizeAligned, CV_8UC3, Scalar(0,255,125));

  IO::WindowIO ioAligned("aligned");
  Mat background = Mat(sizeAligned, CV_8UC3, Scalar(0,10,60));
  for (int i=0; i<dest.size(); i++)
  {
    Triangle destTriangle(dest[i]);
    cout << "-------------------------" << endl;
    cout << "Aligning #[" << i << "]" << endl;
    cout << textures[i].bound.vertices << endl;
    cout << "to => " << endl;
    cout << destTriangle.vertices << endl;
    auto texture_ = textures[i].realignTo(destTriangle, &canvas);
    background = texture_.render(&ioAligned, background, true, true);
  }

  imshow("canvas", canvas);
  
  moveWindow("source", 15, 15);
  moveWindow("aligned", 185, 15);
  moveWindow("canvas", 750, 250);
}

void testAAM()
{
  // cout << "Initialising simple untrained AAMs" << endl;
  // auto canvas = chessPattern(3, Size(CANVAS_SIZE, CANVAS_SIZE));
  // auto canvasClone = canvas.clone();

  // // Initial a mesh shape
  // int nVertices = 32;
  // auto mesh = initialMesh(nVertices);
  // IO::WindowIO ioMesh("mesh");
  // mesh.render(&ioMesh, canvasClone);

  // cout << "... Initialising Appearance model ... " << endl;
  // auto appr = Appearance(mesh, &canvas);
  // cout << "... Appearance model initialised" << endl;
  
  // IO::WindowIO ioAAM("aam");
  // appr.render(&ioAAM, Mat::zeros(canvas.size(), canvas.type()));

  // moveWindow("aam", CANVAS_SIZE+15, 0);
  // waitKey(300);

  // TAOTODO: Apply morphological actions and re-render AAM


}

int main(int argc, char** argv)
{
  cout << MAGENTA << "**********************" << RESET << endl;
  cout << MAGENTA << " Mesh shape testing  "  << RESET << endl;
  cout << MAGENTA << "**********************" << RESET << endl;

  testMeshShape(argv);

  cout << MAGENTA << "**********************" << RESET << endl;
  cout << MAGENTA << " Shape model testing  " << RESET << endl;
  cout << MAGENTA << "**********************" << RESET << endl;

  testShape(argv);

  cout << MAGENTA << "***********************************************" << RESET << endl;
  cout << MAGENTA << " Hit a key to proceed to texture model testing " << RESET << endl;
  cout << MAGENTA << "***********************************************" << RESET << endl;
  waitKey(0);
  destroyAllWindows();

  testTexture(argv);

  cout << MAGENTA << "***********************************************" << RESET << endl;
  cout << MAGENTA << " Hit a key to proceed to AAM testing " << RESET << endl;
  cout << MAGENTA << "***********************************************" << RESET << endl;
  waitKey(0);
  destroyAllWindows();
  
  testAAM();
  waitKey(0);
}