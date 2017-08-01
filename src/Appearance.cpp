#include "Appearance.h"

Appearance::Appearance(const Point2d& p, const Mat& i)
{
  this->setImage(i);
  this->setPos(p);
}

void Appearance::setImage(const Mat& i)
{
  i.copyTo(this->a);
}

void Appearance::setPos(const Point2d& p)
{
  this->pos = p;
}

Appearance Appearance::operator +(const Appearance& that) const
{
  return Appearance(
    this->pos, 
    this->a * that.a);
}

Appearance Appearance::operator *(const double scale) const
{
  return Appearance(this->pos, this->a * scale);
}

Appearance Appearance::createFromLinearCombination(Appearance baseApp, vector<Appearance> apps, vector<double> params)
{
  auto product = baseApp;
  int i = 0;
  for (auto a : apps)
  {
    product = product + a * params[i];
    ++i;
  }
  return product;
}

