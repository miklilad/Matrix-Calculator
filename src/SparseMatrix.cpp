#include "SparseMatrix.h"

double SparseMatrix::At(int x, int y) const
{
  dataPoint point = {x, y, 0};
  const auto& it = std::lower_bound(data.begin(), data.end(), point, Compare);
  if (it == data.end())
    return 0;
  if (it->x == x && it->y == y)
    return it->num;
  else
    return 0;
}

bool SparseMatrix::Compare(const SparseMatrix::dataPoint& d1, const SparseMatrix::dataPoint& d2)
{
  if (d1.x == d2.x)
    return d1.y < d2.y;
  return d1.x < d2.x;
}

void SparseMatrix::SetAt(int x, int y, double val)
{
  dataPoint point = {x, y, val};
  const auto& it = std::lower_bound(data.begin(), data.end(), point, Compare);
  if (it == data.end())
    data.push_back(point);
  else if (it->x == x && it->y == y)
    it->num = val;
  else
    data.insert(it, point);
}

Matrix * SparseMatrix::GetCopy() const
{
  Matrix * copy = new SparseMatrix(width, height);
  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x)
      copy->SetAt(x, y, At(x, y));
  }
  return copy;
}

SparseMatrix::SparseMatrix(int width, int height) : Matrix(width, height)
{

}

void SparseMatrix::Transpose()
{
  for (auto& point:data)
  {
    int num = point.x;
    point.x = point.y;
    point.y = num;
  }
  int num = width;
  width = height;
  height = num;
  std::sort(data.begin(), data.end(), Compare);
}
