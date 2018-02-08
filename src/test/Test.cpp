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
  const int TRAIN_SET_SIZE = 32;
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
  auto cov = alignedShapeSet->covariance(meanShape);
  Mat covResized;
  resize(cov, covResized, Size(CANVAS_SIZE, CANVAS_SIZE), INTER_LINEAR);
  normalize(covResized, covResized, 255, 0, NORM_L2);
  imshow("covariance", covResized);
  moveWindow("covariance", (CANVAS_SIZE+10),(CANVAS_SIZE+50));

  // Calculate PCA shape
  cout << CYAN << "[#] Shape PCA " << RESET << endl;
  auto eigenShape = dynamic_cast<ShapeModelPCA*>(alignedShapeSet->clone()->pca(meanShape, -1));

  // Encode shapes with PCA and measure the estimation errors
  int i = 0;
  cout << GREEN << "[PCA-parameterised shapes]" << RESET << endl;
  for (auto model : alignedShapeSet->getItems())
  {
    const Shape* shape = dynamic_cast<const Shape*>(model);
    cout << CYAN << "... Encoding shape # " << RESET << i << endl;
    auto param = eigenShape->toParam(shape);
    cout << "... Decoding shape # " << i << endl;
    auto encodedShape = eigenShape->toShape(param);
    double error = shape->procrustesDistance(encodedShape);
    cout << "... Estimation error : " << error << endl;
    i++;
  }
}

void testAAMCollection()
{
  const int TRAIN_SET_SIZE = 32;
  const int SHAPE_SIZE     = 5;
  const int MAX_TEXTURE_SIZE = 128;

  Mat backCanvas = Mat::zeros(CANVAS_SIZE, CANVAS_SIZE, CV_8UC3);

  auto aamCollection = initialAppearanceCollection(TRAIN_SET_SIZE, SHAPE_SIZE);

  destroyAllWindows();

  // Normalise rotation of the entire collection
  aamCollection->normaliseRotation();

  auto ioApp = IO::WindowIO("aligned");
  for (auto m : aamCollection->getItems())
  {
    Appearance* app = static_cast<Appearance*>(m);
    app->render(&ioApp, Mat::zeros(app->getShape().getSpannedSize(), CV_8UC3));
    waitKey(100);
  }

  // Reducing the size
  int size = 500;
  cout << CYAN << "[#] Reducing the size to ~ " << RESET << size << endl;
  auto aamCollectionResized = aamCollection->resizeTo(size);

  // Compute procrustes mean of the collection
  auto meanAppearance = dynamic_cast<Appearance*>(aamCollectionResized->procrustesMean());
  auto ioMean = IO::WindowIO("mean");
  meanAppearance->render(&ioMean, Mat::zeros(CANVAS_SIZE, CANVAS_SIZE, CV_8UC3));
  moveWindow("mean", CANVAS_SIZE + 10, 0);
  waitKey(400);

  // Calculate covariance
  cout << CYAN << "[#] Appearance collection covariance " << RESET << endl;
  auto cov = aamCollectionResized->covariance(meanAppearance);

  Mat covResized;
  resize(cov, covResized, Size(CANVAS_SIZE, CANVAS_SIZE), INTER_LINEAR);
  normalize(covResized, covResized, 255, 0, NORM_L2);
  imshow("covariance", covResized);
  moveWindow("covariance", (CANVAS_SIZE+10),(CANVAS_SIZE+50));
  

  // Calculate PCA appearance
  cout << CYAN << "[#] Appearance PCA " << RESET << endl;
  auto eigenAppearance = dynamic_cast<AppearanceModelPCA*>(aamCollectionResized->pca(meanAppearance, MAX_TEXTURE_SIZE));

  // Encode appearances with PCA and measure the estimation errors
  int i = 0;
  cout << GREEN << "[PCA-parameterised appearances]" << RESET << endl;
  auto models = aamCollectionResized->getItems();
  for (auto model : models)
  {
    const Appearance* appearance = dynamic_cast<const Appearance*>(model);
        
    cout << CYAN << "... Encoding appearance # " << RESET << i << endl;
    auto param = eigenAppearance->toParam(appearance);
    cout << "... Decoding appearance # " << i << endl;
    auto encodedAppearance = eigenAppearance->toAppearance(param);
    double error = appearance->procrustesDistance(encodedAppearance);
    cout << "... Estimation error : " << error << endl;
    i++;
  }

  // TAOTODO:
}

void testTexture(char** argv)
{
  cout << GREEN << "Loading sample image" << RESET << endl;
  Mat original = imread("media/numbers.jpg", CV_LOAD_IMAGE_COLOR);
  Rect crop(20, 20, 160, 160);
  Mat plane = original(crop);

  // Source triangle coordinates
  double sourceData[] = {
    0, 0,            // triangle 1
    (double)plane.cols-1, 0,
    0, (double)plane.rows-1,
    (double)plane.cols-1, 0, // triangle 2
    (double)plane.cols-1, (double)plane.rows-1,
    0, (double)plane.rows-1
  };
  Mat source(6, 2, CV_64FC1, sourceData) ;

  // Destination triangle coordinates
  double destData[] = {
    25, 25,   // triangle 1
    200, 75,
    140, 350,
    200, 75,  // triangle 2
    250, 350,
    140, 350
  };
  Mat dest(6, 2, CV_64FC1, destData);

  // Source texture to warp
  Mat p = plane;
  IO::WindowIO ioT("source");
  vector<Texture> textures;
  for (int n=0; n<2; n++)
  {
    int vx[3] = {n*3, n*3+1, n*3+2};
    Texture tx(Triangle(vx[0], vx[1], vx[2]), &source, &plane);
    p = tx.render(&ioT, p, true, true);
    textures.push_back(tx);
  }
  
  // Re-align to the new triangles & render
  Size sizeAligned(500, 500);
  Mat canvas = Mat(sizeAligned, CV_8UC3, Scalar(0,255,125));

  IO::WindowIO ioAligned("aligned");
  Mat background = Mat(sizeAligned, CV_8UC3, Scalar(0,10,60));
  for (int n=0; n<2; n++)
  {
    cout << "Drawing triangle #" << n << endl;
    int vx[3] = {n*3, n*3+1, n*3+2};
    Triangle destTriangle(vx[0], vx[1], vx[2]);
    auto texture_ = textures[n].realignTo(destTriangle, &dest, &canvas);
    background = texture_.render(&ioAligned, background, true, true);
  }

  imshow("canvas", canvas);
  
  moveWindow("source", 15, 15);
  moveWindow("aligned", 185, 15);
  moveWindow("canvas", 750, 250);
}

void testAppearance()
{
  cout << "Initialising simple untrained appearances" << endl;
  auto canvas = chessPattern(3, Size(CANVAS_SIZE, CANVAS_SIZE));
  auto canvasClone = canvas.clone();

  // Initial a mesh shape
  int nVertices = 32;
  auto mesh = initialMesh(nVertices);
  IO::WindowIO ioMesh("mesh");
  mesh.render(&ioMesh, canvasClone);

  cout << "... Initialising Appearance model ... " << endl;
  auto appr = Appearance(mesh, canvas);
  cout << "... Appearance model initialised" << endl;
  
  IO::WindowIO ioAAM("aam");
  appr.render(&ioAAM, Mat::zeros(canvas.size(), canvas.type()));

  moveWindow("aam", CANVAS_SIZE+15, 0);
  waitKey(300);

  // Resize
  int newSize = 500;
  cout << "... Resizing Appearance model ..." << endl;
  appr.resizeTo(newSize);
  IO::WindowIO ioResized("resized");
  Mat canvasResized = Mat::zeros(newSize, newSize, CV_8UC3);
  rectangle(canvasResized, appr.getShape().getBound(), Scalar(240,200,000), 1);
  appr.render(&ioResized, canvasResized);

  moveWindow("resized", (CANVAS_SIZE+15)*2, 0);
  waitKey(300);
}

int main(int argc, char** argv)
{
  cout << MAGENTA << "**********************" << RESET << endl;
  cout << MAGENTA << " Mesh shape testing  "  << RESET << endl;
  cout << MAGENTA << "**********************" << RESET << endl;

  // testMeshShape(argv); TAODEBUG:

  cout << MAGENTA << "**********************" << RESET << endl;
  cout << MAGENTA << " Shape model testing  " << RESET << endl;
  cout << MAGENTA << "**********************" << RESET << endl;

  // testShape(argv);

  cout << MAGENTA << "***********************************************" << RESET << endl;
  cout << MAGENTA << " Hit a key to proceed to texture model testing " << RESET << endl;
  cout << MAGENTA << "***********************************************" << RESET << endl;
  // waitKey(2000);
  // destroyAllWindows();

  // testTexture(argv);

  cout << MAGENTA << "***********************************************" << RESET << endl;
  cout << MAGENTA << " Hit a key to proceed to appearance testing " << RESET << endl;
  cout << MAGENTA << "***********************************************" << RESET << endl;
  // waitKey(2000);
  // destroyAllWindows();
  
  // testAppearance();

  cout << MAGENTA << "***********************************************" << RESET << endl;
  cout << MAGENTA << " Hit a key to proceed to AAM collection test" << RESET << endl;
  cout << MAGENTA << "***********************************************" << RESET << endl;
  // waitKey(2000);
  // destroyAllWindows();

  testAAMCollection();
  waitKey(0);
}