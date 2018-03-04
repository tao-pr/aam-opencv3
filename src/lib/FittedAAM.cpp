#include "FittedAAM.h"

FittedAAM::FittedAAM(const FittedAAM& another)
{
  this->pcaShape = another.pcaShape;
  this->pcaAppearance = another.pcaAppearance;
  this->shapeParam = another.shapeParam;
  this->appearanceParam = another.appearanceParam;
  this->centre = another.centre;
  this->scale = another.scale;
}

void FittedAAM::setCentre(const Point2d& p)
{

}

void FittedAAM::setScale(const double& s)
{

}

void FittedAAM::setShapeParam(const Mat& param)
{

}

void FittedAAM::setAppearanceParam(const Mat& param)
{

}

Appearance FittedAAM::toAppearance()
{

}

MeshShape FittedAAM::toShape()
{

}

double FittedAAM::measureError(const Mat& sample)
{

}

void FittedAAM::drawOverlay(Mat& canvas)
{

}