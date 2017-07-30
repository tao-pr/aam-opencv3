#include "Appearance.h"

Appearace::Appearance(const Point2D64f& p, const Mat& i)
{
  this->setImage(i);
  this->setPos(p);
}

void Apppearance::setImage(const Mat& i)
{
  i.copyTo(this->a);
}

void Appearance::setPos(const Point2D64f& p)
{
  this->pos = p;
}

Appearance Appearance::operator +(Appearance& that)
{
  return Appearance(
    0.5*this->pos + 0.5*that.pos, 
    this->a * that.a);
}

Appearance Appearance::operator *(const double scale)
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

