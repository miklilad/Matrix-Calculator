#include "Calculator.h"

const int BRIGHTNESSCOUNT = 3;
const int COLORCOUNT = 7;

void Calculator::PrintMatrix(Matrix * m, Matrix * colors) const
{
  int x = m->GetWidth();
  int y = m->GetHeight();
  std::vector<int> max;
  for (int i = 0; i < x; ++i)
  {
    max.push_back(DoubleLength(m->At(i, 0)));
    for (int j = 1; j < y; ++j)
    {
      int length = DoubleLength(m->At(i, j));
      if (length > max[i])
        max[i] = length;
    }
  }
  if (colors == nullptr || !m->SameSize(*colors))
  {
    for (int i = 0; i < y; ++i)
    {
      for (int j = 0; j < x; ++j)
        os << std::setw(max[j] + 1) << m->At(j, i);
      os << std::endl;
    }
  }
  else
  {
    for (int i = 0; i < y; ++i)
    {
      for (int j = 0; j < x; ++j)
      {
        DoubleToSetColor(colors->At(j, i));
        os << std::setw(max[j] + 1) << m->At(j, i);
        OsReset();
      }
      os << std::endl;
    }
  }
}

Calculator::Calculator(std::ostream& os) : os(os)
{

}

Matrix * Calculator::GEM(const Matrix& m, bool commentary = false) const
{
  Matrix * copy = m.GetCopy();
  int width = copy->GetWidth();
  int height = copy->GetHeight();
  DenseMatrix colors(width, height);

  int y = 0;
  for (int x = 0; x < width; ++x)
  {
    int pivot = FindPivot(*copy, x, y);
    if (pivot == -1)
    {
      if (commentary)
        for (int yy = y; yy < height; ++yy)
          colors.SetAt(x, yy, 20);
      continue;
    }
    if (pivot != y)
    {
      copy->SwapRows(y, pivot);
      if (commentary)
      {
        OsBold();
        os << "r" << y + 1 << "<->" << "-r" << pivot + 1 << std::endl;
        OsReset();
        Matrix * colorCopy = colors.GetCopy();
        for (int i = 0; i < width; ++i)
        {
          colorCopy->SetAt(i, y, 10);
          colorCopy->SetAt(i, pivot, 10);
        }
        PrintMatrix(copy, colorCopy);
        os << "---------------------------------" << std::endl;
        delete colorCopy;
      }
      pivot = y;
    }
    int yy = y + 1;
    while (yy < height)
    {
      double ratio = copy->At(x, yy) / copy->At(x, pivot);
      copy->SetAt(x, yy, 0);
      for (int xx = x + 1; xx < width; ++xx)
        copy->SetAt(xx, yy, copy->At(xx, pivot) * -ratio + copy->At(xx, yy));
      if (commentary && ratio != 0)
      {
        colors.SetAt(x, yy, 20);

        OsBold();
        os << "r" << yy + 1 << " = " << -ratio << "*r"
           << pivot + 1 << " + r" << yy + 1 << std::endl;
        OsReset();
      }
      yy++;
    }
    y++;
    if (commentary)
    {
      colors.SetAt(x, pivot, 9);
      PrintMatrix(copy, &colors);
      os << "---------------------------------" << std::endl;
    }
  }
  return copy;
}

double Calculator::Abs(double num) const
{
  return num > 0 ? num : num * -1;
}

Calculator::~Calculator()
{
  for (const auto& x:matricies)
    delete x.second;
}

int Calculator::DoubleLength(double num) const
{
  std::ostringstream oss;
  oss << num;
  std::string text = oss.str();
  return text.length();
}

void Calculator::PrintVariable(const std::string& var) const
{
  const auto& m = matricies.find(var);
  if (m == matricies.end())
  {
    os << "Variable not declared!" << std::endl;
    return;
  }
  PrintMatrix(m->second);
}

void Calculator::OsBold() const
{
  os << "\033[1m";
}

void Calculator::OsFaint() const
{
  os << "\033[2m";
}

void Calculator::OsReset() const
{
  os << "\033[0m";
}

void Calculator::OsSetColor(Calculator::COLORS color, int brightness) const
{
  switch (color)
  {
    case RED:     os << "\033[91m"; break;
    case GREEN:   os << "\033[92m"; break;
    case YELLOW:  os << "\033[93m"; break;
    case BLUE:    os << "\033[94m"; break;
    case MAGENTA: os << "\033[95m"; break;
    case CYAN:    os << "\033[96m"; break;
    default: break;
  }
  switch (brightness)
  {
    case 1: OsBold();
      break;
    case 2: OsFaint();
      break;
    default: break;
  }
}

void Calculator::DoubleToSetColor(double num) const
{
  num += 0.5;
  int brightness = 0;
  for (; brightness < BRIGHTNESSCOUNT; ++brightness)
  {
    if (num > COLORCOUNT)
      num -= COLORCOUNT;
    else
      break;
  }
  OsSetColor(static_cast<COLORS>(num), brightness);
}

int Calculator::FindPivot(const Matrix& m, int col, int startIndex) const
{
  int height = m.GetHeight();
  int pivot = -1;
  while (startIndex < height && m.At(col, startIndex) == 0 )
    startIndex++;
  if (startIndex < height)
    pivot = startIndex;
  startIndex++;
  while (startIndex < height)
  {
    if (Abs(m.At(col, startIndex)) < Abs(m.At(col, pivot)) && m.At(col, startIndex) != 0)
      pivot = startIndex;
    startIndex++;
  }
  return pivot;
}

int Calculator::Rank(const Matrix& m) const
{
  Matrix * gemed = GEM(m);
  int rank = 0;
  int x = 0, y = 0;
  bool yInced = false;
  int width = gemed->GetWidth();
  int height = gemed->GetHeight();
  while (x < width && y < height)
  {
    if (gemed->At(x, y) == 0)
    {
      x++;
      yInced = false;
    }
    else if (!yInced)
    {
      yInced = true;
      y++;
      rank++;
    }
    else
      y++;
  }
  delete gemed;
  return rank;
}

double Calculator::Determinant(const Matrix& m) const
{
  Matrix * gemed = GEM(m);
  double determinant = gemed->At(0, 0);
  for (int i = 1; i < gemed->GetWidth(); ++i)
    determinant *= gemed->At(i, i);
  delete gemed;
  return determinant;
}

Matrix * Calculator::Merge(const Matrix& m1, const Matrix& m2, int direction) const
{
  if ((direction == 1 && m1.GetHeight() != m2.GetHeight()) ||
      (direction == 2 && m1.GetWidth() != m2.GetWidth()) ||
      (direction != 1 && direction != 2 && m1.GetWidth() != m2.GetWidth() && m1.GetHeight() != m2.GetHeight()))
  {
    OsBold();
    OsSetColor(RED);
    os << "Dimensions don't match!" << std::endl;
    OsReset();
    return nullptr;
  }
  Matrix * newMatrix;
  if (typeid(SparseMatrix) == typeid(m1) &&
      typeid(SparseMatrix) == typeid(m2))
  {
    if (direction == 1)
      newMatrix = new SparseMatrix(m1.GetWidth() + m2.GetWidth(), m1.GetHeight());
    else if (direction == 2)
      newMatrix = new SparseMatrix(m1.GetWidth(), m1.GetHeight() + m2.GetHeight());
    else if (m1.GetHeight() == m2.GetHeight())
    {
      newMatrix = new SparseMatrix(m1.GetWidth() + m2.GetWidth(), m1.GetHeight());
      direction = 1;
    }
    else
    {
      newMatrix = new SparseMatrix(m1.GetWidth(), m1.GetHeight() + m2.GetHeight());
      direction = 2;
    }
  }
  else
  {
    if (direction == 1)
      newMatrix = new DenseMatrix(m1.GetWidth() + m2.GetWidth(), m1.GetHeight());
    else if (direction == 2)
      newMatrix = new DenseMatrix(m1.GetWidth(), m1.GetHeight() + m2.GetHeight());
    else if (m1.GetHeight() == m2.GetHeight())
    {
      newMatrix = new DenseMatrix(m1.GetWidth() + m2.GetWidth(), m1.GetHeight());
      direction = 1;
    }
    else
    {
      newMatrix = new DenseMatrix(m1.GetWidth(), m1.GetHeight() + m2.GetHeight());
      direction = 2;
    }
  }
  if (direction == 1)
  {
    for (int x = 0; x < m1.GetWidth(); ++x)
      for (int y = 0; y < m1.GetHeight(); ++y)
        newMatrix->SetAt(x, y, m1.At(x, y));
    for (int x = 0; x < m2.GetWidth(); ++x)
      for (int y = 0; y < m2.GetHeight(); ++y)
        newMatrix->SetAt(x + m1.GetWidth(), y, m2.At(x, y));
  }
  else
  {
    for (int x = 0; x < m1.GetWidth(); ++x)
      for (int y = 0; y < m1.GetHeight(); ++y)
        newMatrix->SetAt(x, y, m1.At(x, y));
    for (int x = 0; x < m2.GetWidth(); ++x)
      for (int y = 0; y < m2.GetHeight(); ++y)
        newMatrix->SetAt(x, y + m1.GetHeight(), m2.At(x, y));
  }
  return newMatrix;
}

Matrix * Calculator::Split(const Matrix& m, int x, int y, int width, int height) const
{
  Matrix * splitted = new DenseMatrix(width, height);
  int zeroCount = 0;
  for (int i = 0; i < width; ++i)
  {
    for (int j = 0; j < height; ++j)
    {
      double val = m.At(x + i, y + j);
      splitted->SetAt(i, j, val);
      if (val == 0)
        zeroCount++;
    }
  }
  double referenceRatio = (double) (sizeof(double) + 2 * sizeof(int)) / sizeof(double);
  double ratio = (double) width * height / zeroCount;
  if (ratio < referenceRatio)
  {
    Matrix * sparse = new SparseMatrix(x, y);
    for (int i = 0; i < x; ++i)
    {
      for (int j = 0; j < y; ++j)
      {
        double val = splitted->At(i, j);
        if (val != 0)
          sparse->SetAt(i, j, val);
      }
    }
    delete splitted;
    return sparse;
  }
  else
    return splitted;
}

Matrix * Calculator::Add(const Matrix& m1, const Matrix& m2) const
{
  if (m1.GetWidth() != m2.GetWidth() || m1.GetHeight() != m2.GetHeight())
    return nullptr;
  Matrix * result;
  if (typeid(SparseMatrix) == typeid(m1) && typeid(SparseMatrix) == typeid(m2))
    result = new SparseMatrix(m1.GetWidth(), m1.GetHeight());
  else
    result = new DenseMatrix(m1.GetWidth(), m1.GetHeight());
  for (int x = 0; x < m1.GetWidth(); ++x)
    for (int y = 0; y < m1.GetHeight(); ++y)
      result->SetAt(x, y, m1.At(x, y) + m2.At(x, y));
  return result;
}

Matrix * Calculator::Multiply(const Matrix& m1, const Matrix& m2) const
{
  if (m2.GetHeight() != m1.GetWidth())
    return nullptr;
  Matrix * result;
  if (typeid(SparseMatrix) == typeid(m1) && typeid(SparseMatrix) == typeid(m2))
    result = new SparseMatrix(m2.GetWidth(), m1.GetHeight());
  else
    result = new DenseMatrix(m2.GetWidth(), m1.GetHeight());
  for (int x = 0; x < result->GetWidth(); ++x)
  {
    for (int y = 0; y < result->GetHeight(); ++y)
    {
      double val = 0;
      for (int i = 0; i < m1.GetWidth(); ++i)
        val += m1.At(i, y) * m2.At(x, i);
      result->SetAt(x, y, val);
    }
  }
  return result;
}

Matrix * Calculator::Inverse(const Matrix& m) const
{
  int size = m.GetWidth();
  if (size != m.GetHeight())
    std::__throw_invalid_argument("Not a square matrix!");
  SparseMatrix identity(size, size);
  for (int i = 0; i < size; ++i)
    identity.SetAt(i, i, 1);
  Matrix * extended = Merge(m, identity, 1);
  Matrix * gemed = GEM(*extended);
  delete extended;
  for (int i = size - 1; i >= 0; --i)
  {
    double pivot = gemed->At(i, i);
    if (pivot == 0)
    {
      delete gemed;
      std::__throw_invalid_argument("Matrix isn't invertible!");
    }
    for (int y = i - 1; y >= 0; --y)
    {
      double ratio = gemed->At(i, y) / pivot;
      for (int x = 0; x < size * 2; ++x)
        gemed->SetAt(x, y, gemed->At(x, i) * -ratio + gemed->At(x, y));
    }
    for (int x = 0; x < size * 2; ++x)
      gemed->SetAt(x, i, gemed->At(x, i) / pivot);
  }
  Matrix * splitted = Split(*gemed, size, 0, size, size);
  delete gemed;
  return splitted;
}
