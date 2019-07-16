#include <stdexcept>
#include "Matrix.h"


void Matrix::Transpose()
{

}

Matrix::Matrix(int width, int height) : width(width), height(height)
{
  if (width < 1 || height < 1)
    throw std::exception();
}

void Matrix::SetAt(int x, int y, double val)
{}

double Matrix::At(int x, int y) const
{
  return 0;
}

Matrix::~Matrix()
{

}

int Matrix::GetHeight() const
{
  return height;
}

int Matrix::GetWidth() const
{
  return width;
}

bool Matrix::SameSize(const Matrix& other) const
{
  return width == other.GetWidth() && height == other.GetHeight();
}

Matrix * Matrix::GetCopy() const
{
  return new Matrix(width, height);
}

void Matrix::SwapRows(int row1, int row2)
{
  if (row1 >= height || row2 >= height || row1 < 0 || row2 < 0)
    throw std::exception();
  for (int x = 0; x < width; ++x)
  {
    double num = At(x, row1);
    SetAt(x, row1, At(x, row2));
    if (num != 0)
      SetAt(x, row2, -num);
    else
      SetAt(x, row2, 0);
  }
}

void Matrix::ScalarMul(double num)
{
  for (int x = 0; x < width; ++x)
    for (int y = 0; y < height; ++y)
      SetAt(x, y, num * At(x, y));
}
