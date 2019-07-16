/**
* @file         DenseMatrix.h
* @date         9.6.2019
* @brief        Definition of the DenseMatrix
* @author       miklilad
*/
#ifndef SEM_DENSEMATRIX_H
#define SEM_DENSEMATRIX_H

#include "Matrix.h"

/**
* @class    DenseMatrix
* @brief    Efficient storage for dense matricies
* @details  Faster but bigger than SparseMatrix. Uses 2D array to store data.
*/
class DenseMatrix : public Matrix
{
  double ** data;

public:
  DenseMatrix(int width,int height);
  double At(int x, int y) const override;

  void SetAt(int x, int y, double val) override;

  Matrix * GetCopy() const override;

  void Transpose() override;

  ~DenseMatrix() override ;
};


#endif
