/**
* @file         SparseMatrix.h
* @date         9.6.2019
* @brief        Definition of the SparseMatrix
* @author       miklilad
*/
#ifndef SEM_SPARSEMATRIX_H
#define SEM_SPARSEMATRIX_H

#include "Matrix.h"
#include <vector>
#include <algorithm>

/**
* @class    SparseMatrix
* @brief    Efficient storage for sparse matricies
* @details  Lighter but slower than DenseMatrix. Uses vector of dataPoint to store data.
*/
class SparseMatrix : public Matrix
{
  struct dataPoint
  {
    int x, y;
    double num;
  };

  std::vector<dataPoint> data;

public:

  static bool Compare(const dataPoint& d1, const dataPoint& d2);

  SparseMatrix(int width, int height);

  double At(int x, int y) const override;

  void SetAt(int x, int y, double val) override;

  Matrix * GetCopy() const override;

  void Transpose() override;

};


#endif
