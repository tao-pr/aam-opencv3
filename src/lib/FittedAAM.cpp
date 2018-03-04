#include "FittedAAM.h"

FittedAAM::FittedAAM(const FittedAAM& another)
{
  this->pcaShape = another.pcaShape;
  this->pcaAppearance = another.pcaAppearance;
  this->shapeParam = another.shapeParam;
  this->appearanceParam = another.appearanceParam;
  this->centre = another.centre;
}

void FittedAAM::setCentre(const Point2d& p)
{
  this->centre = p;
}

const double FittedAAM::getMeanShapeScale()
{
  return this->pcaAppearance.getScale();
}

void FittedAAM::setShapeParam(const Mat& param)
{
  param.copyTo(this->shapeParam);
}

void FittedAAM::setAppearanceParam(const Mat& param)
{
  param.copyTo(this->appearanceParam);
}

Appearance* FittedAAM::toAppearance()
{
  return this->pcaAppearance.toAppearance(this->appearanceParam);
}

MeshShape* FittedAAM::toShape()
{
  return this->pcaShape.toShape(this->shapeParam);
}

double FittedAAM::measureError(const Mat& sample)
{

}

void FittedAAM::drawOverlay(Mat& canvas)
{

}