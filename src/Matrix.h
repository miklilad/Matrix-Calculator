/**
* @file         Matrix.h
* @date         9.6.2019
* @brief        Definition of the Matrix
* @author       miklilad
*/
#ifndef SEM_MATRIX_H
#define SEM_MATRIX_H

/**
* @class    Matrix
* @brief    Mathematical object
*/
class Matrix
{
protected:
  int width;
  int height;

public:
  Matrix(int width, int height);

  /**
  * @fn        GetHeight
  * @brief     Height getter
  */
  int GetHeight() const;

  /**
  * @fn        GetHeight
  * @brief     Width getter
  */
  int GetWidth() const;

  /**
  * @fn        At
  * @returns   Value at x,y position
  */
  virtual double At(int x, int y) const;

  /**
  * @fn        SetAt
  * @brief     Sets value at x,y position
  */
  virtual void SetAt(int x, int y, double val);

  /**
  * @fn        GetCopy
  * @returns   Copy of the matrix
  */
  virtual Matrix * GetCopy() const;

  /**
  * @fn        SwapRows
  * @brief     Swaps 2 rows in the matrix
  */
  void SwapRows(int row1, int row2);

  /**
  * @fn        ScalarMul
  * @brief     Multiplies the matrix by given scalar
  */
  void ScalarMul(double num);

  /**
  * @fn        SameSize
  * @brief     Checks, if 2 matricies are the same size
  */
  bool SameSize(const Matrix& other) const;

  /**
  * @fn        Transpose
  * @brief     Transposes the matrix
  */
  virtual void Transpose();

  virtual ~Matrix();
};

#endif
