/**
* @file         Calculator.h
* @date         9.6.2019
* @brief        Definition of the Calculator
* @author       miklilad
*/
#ifndef SEM_CALC
#define SEM_CALC

#include <map>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <locale>
#include <vector>
#include <typeinfo>
#include "Matrix.h"
#include "SparseMatrix.h"
#include "DenseMatrix.h"

/**
* @class    Calculator
* @brief    Matrix calculator
* @details  Calculator stores matrix variables and operates over them
*/
class Calculator
{
  std::ostream& os;

  enum COLORS
  {
    RED = 1, GREEN, YELLOW, BLUE, MAGENTA, CYAN
  };

  /**
  * @fn        DoubleLength
  * @returns   Length of string of double
  */
  int DoubleLength(double num) const;

  /**
  * @fn        Abs
  * @returns   Absolute value of num
  */
  double Abs(double num) const;

  /**
  * @fn        FindPivot
  * @param     col - collum of matrix m
  * @param     startIndex - Takes into account only indexes from startIndex and onward
  * @returns   Index in col of m, that has closet number to zero excluding zero
  * @returns   or -1 if no such number was found
  */
  int FindPivot(const Matrix& m, int col, int startIndex) const;

  /**
  * @fn        OsSetColor
  * @brief     Sets color and brightness to os
  * @param     color - From COLORS
  * @param     brightness == 1 - Bold
  * @param     brightness == 2 - Faint, anything else is normal brightness
  */
  void OsSetColor(Calculator::COLORS color, int brightness = 0) const;

  /**
  * @fn        DoubleToSetColor
  * @brief     Sets color and brightness to os decoded from num
  * @param     num - Encoded color
  */
  void DoubleToSetColor(double num) const;

  /**
  * @fn        OsBold
  * @brief     Makes os bold
  */
  void OsBold() const;

  /**
  * @fn        OsFaint
  * @brief     Makes os faint
  */
  void OsFaint() const;

  /**
  * @fn        OsReset
  * @brief     Resets os's format flags
  */
  void OsReset() const;

public:
  std::map<std::string, Matrix *> matricies;

  Calculator(std::ostream& os = std::cout);

  /**
  * @fn        GEM
  * @brief     Gauss-elimination method
  * @param     commentary - True to comment the steps
  * @returns   Pointer to gauss-eliminated matrix
  */
  Matrix * GEM(const Matrix& m, bool commentary) const;

  /**
  * @fn        Rank
  * @details   Gems the matrix and then counts number of pivots
  * @returns   Rank of a matrix
  */
  int Rank(const Matrix& m) const;

  /**
  * @fn        Determinant
  * @details   Gems the matrix and multiplies its diagonal
  * @returns   Determinant of a matrix
  */
  double Determinant(const Matrix& m) const;

  /**
  * @fn        PrintVariable
  * @brief     Prints the variable from matricies to os
  */
  void PrintVariable(const std::string& var) const;

  /**
  * @fn        PrintMatrix
  * @brief     Prints the matrix to os
  * @param     m - Matrix to be printed
  * @param     colors - matrix that encodes color values
  * @details   Optional color matrix may be provided to print the matrix in color.
  * @details   Double to color is used to convert the values of color matrix to color
  */
  void PrintMatrix(Matrix * m, Matrix * colors = nullptr) const;

  /**
  * @fn        Merge
  * @brief     Merges 2 matricies together
  * @param     direction == 1 - Horizontally
  * @param     direction == 2 - Vertically, anything else and the calculator will try to figure it out
  * for itself, preferring the horizontal direction
  */
  Matrix * Merge(const Matrix& m1, const Matrix& m2, int direction = 0) const;

  /**
  * @fn        Split
  * @brief     Creates a submatrix of a matrix
  * @param     x,y - Position of upper-left corner of the submatrix
  * @param     width, height - Dimensions of the submatrix
  * @returns   Either the submatrix pointer or nullptr, if the dimensions of the submatrix
  * @returns   are outside the area of the original matrix
  */
  Matrix * Split(const Matrix& m, int x, int y, int width, int height) const;

  /**
  * @fn        Add
  * @brief     Adds 2 matricies together
  * @returns   Pointer to the new matrix
  */
  Matrix * Add(const Matrix& m1, const Matrix& m2) const;

  /**
  * @fn        Multiply
  * @brief     Multiply 2 matricies together
  * @returns   Pointer to the new matrix
  */
  Matrix * Multiply(const Matrix& m1, const Matrix& m2) const;

  /**
  * @fn        Inverse
  * @brief     Creates an inverse matrix of square matrix
  * @returns   Pointer to the new matrix
  */
  Matrix * Inverse(const Matrix& m) const;

  ~Calculator();
};

#endif