#include "Parser.h"

bool Parser::Read()
{
  char c = 'a';
  std::string line;
  while (is.get(c) && c != '\n')
    line.push_back(c);
  if (line == "q" || is.eof())
    return false;
  Parse(line);
  return true;
}

void Parser::Run()
{
  while (Read())
    continue;
}

void Parser::Parse(const std::string& line)
{
  std::istringstream iss;
  iss.str(line);
  std::string command, variable, saveTo;
  command = ReadAlpha(iss);
  if (CheckAndGetChar(iss, '='))
  {
    saveTo = command;
    if (saveTo.empty())
    {
      WriteError("Wrong variable name!");
      return;
    }
    command = ReadAlpha(iss);
  }
  command = ToLower(command);

  if (command == "scan")
    ParseScan(iss, saveTo);
  else if (command == "gem")
    ParseGem(iss, saveTo);
  else if (command == "transpose" && saveTo.empty())
    ParseTranspose(iss);
  else if ((command == "print" || command == "p") && saveTo.empty())
    ParsePrint(iss);
  else if (command == "rank" && saveTo.empty())
    ParseRank(iss);
  else if (command == "split")
    ParseSplit(iss, saveTo);
  else if (command == "merge")
    ParseMerge(iss, saveTo);
  else if (command == "determinant" && saveTo.empty())
    ParseDeterminant(iss);
  else if (command == "inverse")
    ParseInverse(iss, saveTo);
  else if (calc.matricies.find(command) != calc.matricies.end())
    ParseAddMulSub(iss, command, saveTo);
  else if (!command.empty() || !saveTo.empty())
  {
    WriteError("Wrong input!");
    return;
  }
}

void Parser::Scan(const std::string& name, int width, int height)
{
  int zeroCount = 0;
  delete calc.matricies[name];
  Matrix * dense;
  try
  {
    dense = new DenseMatrix(width, height);
  }
  catch (std::exception& e)
  {
    WriteError("Wrong size!");
    return;
  }
  for (int i = 0; i < height; ++i)
  {
    for (int j = 0; j < width; ++j)
    {
      double num;
      do
      {
        if (is.fail())
        {
          is.clear();
          is.ignore(500, '\n');
        }
        os << "Row " << i + 1 << ", Col " << j + 1 << ": ";
        is >> num;
      } while (is.fail() && !is.eof());
      dense->SetAt(j, i, num);
      if (num == 0)
        zeroCount++;
    }
  }
  double referenceRatio = (double) (sizeof(double) + 2 * sizeof(int)) / sizeof(double);
  double ratio = (double) dense->GetHeight() * dense->GetWidth() / zeroCount;
  if (ratio < referenceRatio)
  {
    Matrix * sparse = new SparseMatrix(width, height);
    for (int i = 0; i < width; ++i)
    {
      for (int j = 0; j < height; ++j)
      {
        double val = dense->At(i, j);
        if (val != 0)
          sparse->SetAt(i, j, val);
      }
    }
    calc.matricies[name] = sparse;
    delete dense;
  }
  else
    calc.matricies[name] = dense;
}

char Parser::ReadArgument(std::istringstream& iss) const
{
  char c = 0;
  GetRidOfSpaces(iss);
  if (!CheckAndGetChar(iss, '-'))
    return 0;
  if (std::isalpha(iss.peek()))
    iss.get(c);
  if (!std::isspace(iss.peek()))
    return 1;
  return c;
}

std::string Parser::ToLower(const std::string& text) const
{
  std::string lower;
  for (auto x:text)
    lower.push_back(tolower(x));
  return lower;
}

void Parser::GetRidOfSpaces(std::istringstream& iss) const
{
  while (std::isspace(iss.peek()))
    iss.get();
}

std::string Parser::ReadAlpha(std::istringstream& iss) const
{
  std::string var;
  char c;
  GetRidOfSpaces(iss);
  while (std::isalpha(iss.peek()))
  {
    iss.get(c);
    var.push_back(c);
  }
  return var;
}

int Parser::ReadNum(std::istringstream& iss) const
{
  std::string var;
  char c;
  GetRidOfSpaces(iss);
  while (std::isdigit(iss.peek()))
  {
    iss.get(c);
    var.push_back(c);
  }
  if (!var.empty())
    return std::stoi(var);
  else
    return -1;
}

bool Parser::CheckAndGetChar(std::istringstream& iss, char c) const
{
  GetRidOfSpaces(iss);
  if (iss.peek() == c)
  {
    iss.get();
    return true;
  }
  return false;
}

bool Parser::EndOfCommand(std::istringstream& iss) const
{
  char c = 'a';
  iss >> c;
  if (c == '\n' && iss.good())
    iss >> c;
  return iss.fail();
}

Parser::Parser(std::istream& is, std::ostream& os) : is(is), os(os), calc(os)
{

}

void Parser::ParseScan(std::istringstream& iss, std::string& saveTo)
{
  int x, y;
  try
  {
    if (saveTo.empty())
    {
      saveTo = ReadAlpha(iss);
      if (saveTo.empty())
        throw "Wrong variable name!";
    }
    if (!CheckAndGetChar(iss, '['))
      throw "Wrong Syntax!";
    x = ReadNum(iss);
    if (x == -1)
      throw "Wrong Size!";
    if (!CheckAndGetChar(iss, ','))
      throw "Wrong Syntax!";
    y = ReadNum(iss);
    if (y == -1)
      throw "Wrong Size!";
    if (!CheckAndGetChar(iss, ']'))
      throw "Wrong Syntax!";
    if (!EndOfCommand(iss))
      throw "Command not properly ended!";
  }
  catch (const char * msg)
  {
    WriteError(msg);
    return;
  }
  catch (std::out_of_range& e)
  {
    WriteError("Number out of range!");
    return;
  }
  Scan(saveTo, x, y);
}

void Parser::ParseGem(std::istringstream& iss, const std::string& saveTo)
{
  std::string variable;
  bool commentary = false;
  try
  {
    char c = ReadArgument(iss);
    if (c == 'v')
      commentary = true;
    else if (c == 1)
      throw "Syntax Error";
    else if (c != 0)
      throw "Unknown argument!";
    variable = ReadAlpha(iss);
    if (variable.empty())
      throw "Wrong variable name!";
    if (!EndOfCommand(iss))
      throw "Command not properly ended!";
    if (!CheckVariableUsage(variable))
      return;
  }
  catch (const char * msg)
  {
    WriteError(msg);
    return;
  }
  Matrix * gemed = calc.GEM(*calc.matricies[variable], commentary);
  if (saveTo.empty() && !commentary)
    calc.PrintMatrix(gemed);
  if (!saveTo.empty())
  {
    delete calc.matricies[saveTo];
    calc.matricies[saveTo] = gemed;
  }
  else
    delete gemed;
}

void Parser::ParseTranspose(std::istringstream& iss)
{
  std::string variable = ReadAlpha(iss);
  if (variable.empty())
  {
    WriteError("Wrong variable name!");
    return;
  }
  if (!CheckVariableUsage(variable))
    return;
  if (!EndOfCommand(iss))
  {
    WriteError("Command not properly ended!");
    return;
  }
  calc.matricies[variable]->Transpose();
}

bool Parser::CheckVariableUsage(const std::string& variable) const
{
  if (calc.matricies.find(variable) == calc.matricies.end())
  {
    WriteError("Variable not used!");
    return false;
  }
  return true;
}

void Parser::WriteError(const char * msg) const
{
  os << "\033[1;91m" << msg << "\033[0m" << std::endl;
}

void Parser::ParsePrint(std::istringstream& iss) const
{
  std::string variable = ReadAlpha(iss);
  if (!EndOfCommand(iss))
  {
    WriteError("Command not properly ended!");
    return;
  }
  calc.PrintVariable(variable);
}

void Parser::ParseRank(std::istringstream& iss) const
{
  std::string variable = ReadAlpha(iss);
  if (!CheckVariableUsage(variable))
    return;
  if (!EndOfCommand(iss))
  {
    WriteError("Command not properly ended!");
    return;
  }
  const auto& m = calc.matricies.find(variable);
  os << calc.Rank(*m->second) << std::endl;
}

void Parser::ParseSplit(std::istringstream& iss, const std::string& saveTo)
{
  std::string variable = ReadAlpha(iss);
  int x, y, width, height;
  try
  {
    if (!CheckVariableUsage(variable))
      return;
    if (!CheckAndGetChar(iss, '['))
      throw "Syntax error!";
    width = ReadNum(iss);
    if (!CheckAndGetChar(iss, ','))
      throw "Syntax error!";
    height = ReadNum(iss);
    if (!CheckAndGetChar(iss, ']'))
      throw "Syntax error!";
    if (!CheckAndGetChar(iss, '('))
      throw "Syntax error!";
    x = ReadNum(iss);
    if (!CheckAndGetChar(iss, ','))
      throw "Syntax error!";
    y = ReadNum(iss);
    if (!CheckAndGetChar(iss, ')'))
      throw "Syntax error!";
    if (!EndOfCommand(iss))
      throw "Command not properly ended!";
    if (width == 0 || height == 0)
      throw "Wrong size!";
    if (width == -1 || height == -1 || x == -1 || y == -1)
      throw "Syntax error!";
    if (calc.matricies[variable]->GetHeight() < y + height ||
        calc.matricies[variable]->GetWidth() < x + width)
      throw "Area out of matrix bounds!";
  }
  catch (const char * msg)
  {
    WriteError(msg);
    return;
  }
  Matrix * splitted = calc.Split(*calc.matricies[variable], x, y, width, height);
  if (saveTo.empty())
  {
    calc.PrintMatrix(splitted);
    delete splitted;
  }
  else
  {
    delete calc.matricies[saveTo];
    calc.matricies[saveTo] = splitted;
  }
}

void Parser::ParseMerge(std::istringstream& iss, const std::string& saveTo)
{
  int mergeDirection = 0;
  char c = ReadArgument(iss);
  std::string variable = ReadAlpha(iss);
  std::string variable2 = ReadAlpha(iss);
  try
  {
    if (c == 'v')
      mergeDirection = 2;
    else if (c == 'h')
      mergeDirection = 1;
    else if (c == 1)
      throw "Syntax Error";
    else if (c != 0)
      throw "Unknown argument!";
    if (calc.matricies.find(variable) == calc.matricies.end())
      throw "First matrix not declared";
    if (calc.matricies.find(variable2) == calc.matricies.end())
      throw "Second matrix not declared";
    if (!EndOfCommand(iss))
      throw "Command not properly ended!";
  }
  catch (const char * msg)
  {
    WriteError(msg);
    return;
  }
  Matrix * m = calc.Merge(*calc.matricies[variable], *calc.matricies[variable2], mergeDirection);
  if (m == nullptr)
    return;
  if (saveTo.empty())
  {
    calc.PrintMatrix(m);
    delete m;
  }
  else
  {
    delete calc.matricies[saveTo];
    calc.matricies[saveTo] = m;
  }
}

void Parser::ParseDeterminant(std::istringstream& iss) const
{
  std::string variable = ReadAlpha(iss);
  if (!CheckVariableUsage(variable))
    return;
  if (!EndOfCommand(iss))
  {
    WriteError("Command not ended properly!");
    return;
  }
  const auto& m = calc.matricies.find(variable);
  if (m->second->GetWidth() == m->second->GetHeight())
    os << calc.Determinant(*m->second) << std::endl;
  else
    WriteError("Not a square matrix!");
}

void Parser::ParseAddMulSub(std::istringstream& iss, const std::string& variable, const std::string& saveTo)
{
  char c = 0;
  if (CheckAndGetChar(iss, '+'))
    c = '+';
  else if (CheckAndGetChar(iss, '*'))
    c = '*';
  else if (CheckAndGetChar(iss, '-'))
    c = '-';
  else
  {
    WriteError("Unknown operator!");
    return;
  }
  std::string variable2 = ReadAlpha(iss);
  if (!CheckVariableUsage(variable2))
    return;
  if (!EndOfCommand(iss))
  {
    WriteError("Command not properly ended!");
    return;
  }
  Matrix * result;
  if (c == '+')
    result = calc.Add(*calc.matricies[variable], *calc.matricies[variable2]);
  else if (c == '*')
    result = calc.Multiply(*calc.matricies[variable], *calc.matricies[variable2]);
  else
  {
    Matrix * m = calc.matricies[variable2]->GetCopy();
    m->ScalarMul(-1);
    result = calc.Add(*m, *calc.matricies[variable]);
    delete m;
  }
  if (!result)
  {
    WriteError("Dimensions don't match!");
    return;
  }
  if (saveTo.empty())
  {
    calc.PrintMatrix(result);
    delete result;
  }
  else
  {
    delete calc.matricies[saveTo];
    calc.matricies[saveTo] = result;
  }
}

void Parser::ParseInverse(std::istringstream& iss, const std::string& saveTo)
{
  std::string variable = ReadAlpha(iss);
  if (!CheckVariableUsage(variable))
    return;
  if (!EndOfCommand(iss))
  {
    WriteError("Command not ended properly!");
    return;
  }
  Matrix * m = nullptr;
  try
  {
    m = calc.Inverse(*calc.matricies[variable]);
  }
  catch (const std::invalid_argument& e)
  {
    WriteError(e.what());
    return;
  }
  if (saveTo.empty())
  {
    calc.PrintMatrix(m);
    delete m;
  }
  else
  {
    delete calc.matricies[saveTo];
    calc.matricies[saveTo] = m;
  }
}
