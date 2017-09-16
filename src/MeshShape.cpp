#include "MeshShape.h"

MeshShape::MeshShape(vector<Point2d>& vs)
{
  Shape(vs);
  this->subdiv = Subdiv2D(size);
  for (auto const v : this->vertices)
  {
    this->subdiv.insert(v);
  }
}

void MeshShape::render(IO::GenericIO io, Mat background) const
{
  // TAOTOREVIEW: Utilise OpenGL
  vector<Vec6f> triangles;
  this->subdiv.getTriangleList(triangles);
  vector<Point2d> hull = this->convexHull();
  Mat canvas = Mat(scaledBound.height, scaledBound.width, CV_32FC3);
  canvas.copyFrom(background);

  // Render boundary
  Point2d v0 = hull.front();
  for (auto v : hull)
  {
    line(canvas, v0, v, Scalar(0,0,200), 1, CV_AA);
    v0 = v;
  }

  io.render(canvas);
}

// Shape& Shape::fromAnnotation(const vector<Point2d>& ps, Mat img)
// {
//   // Apply PCA on annotated points
//   Mat rawPoints = Mat(ps.size(), 2, CV_64FC1);
//   for (int i=0; i<ps.size(); ++i)
//   {
//     rawPoints.at(i, 0) = ps[i].x;
//     rawPoints.at(i, 1) = ps[i].y;
//   }

//   int nComponents = min(MAX_SHAPE_COMPONENTS, ps.size()/2);
//   PCA pca(rawPoints, Mat(), PCA::DATA_AS_COLUMN, nComponents);
//   Mat pcaPoints = Mat(nComponents, 2, CV_64FC1);

//   // TAOTODO:
// }