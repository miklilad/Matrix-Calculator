#include "DenseMatrix.h"

double DenseMatrix::At(int x, int y) const
{
  return data[x][y];
}

void DenseMatrix::SetAt(int x, int y, double val)
{
  data[x][y] = val;
}

DenseMatrix::DenseMatrix(int width, int height) : Matrix(width, height)
{
  data = new double * [width];
  for (int i = 0; i < width; ++i)
  {
    data[i] = new double[height];
    for (int j = 0; j < height; ++j)
      data[i][j] = 0;
  }
}

DenseMatrix::~DenseMatrix()
{
  for (int i = 0; i < width; ++i)
    delete[] data[i];
  delete[] data;
}

Matrix * DenseMatrix::GetCopy() const
{
  Matrix * copy = new DenseMatrix(width, height);
  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x)
      copy->SetAt(x, y, At(x, y));
  }
  return copy;
}

void DenseMatrix::Transpose()
{
  double ** transposed;
  transposed = new double * [height];
  for (int i = 0; i < height; ++i)
  {
    transposed[i] = new double[width];
    for (int j = 0; j < width; ++j)
      transposed[i][j] = At(j, i);
  }
  for (int i = 0; i < width; ++i)
    delete[] data[i];
  delete[] data;
  data = transposed;
  int num = width;
  width = height;
  height = num;
}
