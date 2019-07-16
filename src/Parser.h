/**
* @file         Parser.h
* @date         9.6.2019
* @brief        Definition of the Parser
* @author       miklilad
*/
#ifndef SEM_PARSER_H
#define SEM_PARSER_H

#include "Calculator.h"

/**
* @class    Parser
* @brief    Transforms input stream to calculator commands
* @details  Parser passes the commands to calculator. Input/Output stream may
* @details  be any stream defined at the construction of the object
*/
class Parser
{
  std::istream& is;
  std::ostream& os;
  Calculator calc;

  /**
  * @fn        Read
  * @brief     Reads input from input stream until '\\n' is reached and saves it to string.
  * @returns   False , if extracted  string is "q" or eof was reached. Returns True otherwise
  */
  bool Read();

  /**
  * @fn        Parse
  * @brief     Takes string from Read() and parses it to calculator commands
  * @param     line - Text to be parsed taken from Read()
  * @details   Converts line to string stream. Reads iss until any non-alphabetic
  * @details   character was reached and either takes it as a variable, into which following command
  * @details   is supposed to be saved, if following non-space character is '=',
  * @details   or takes it as a command and prints the result to os.
  */
  void Parse(const std::string& line);

  /**
  * @fn        ParseScan
  * @brief     Reads the rest of iss, parses and executes command
  * @param     iss - Stream from which the commands are parsed
  * @param     saveTo - Variable name, into which the result is to be saved
  * @details   Executes the command, if the command's syntax was respected
  * @details   and prints the result to os or saves it to calc, if saveTo isn't empty.
  */
  void ParseScan(std::istringstream& iss, std::string& saveTo);

  /**
  * @fn        ParseGem
  * @brief     Reads the rest of iss, parses and executes command
  * @param     iss - Stream from which the commands are parsed
  * @param     saveTo - Variable name, into which the result is to be saved
  * @details   Executes the command, if the command's syntax was respected
  * @details   and prints the result to os or saves it to calc, if saveTo isn't empty.
  */
  void ParseGem(std::istringstream& iss, const std::string& saveTo);

  /**
  * @fn        ParseTranspose
  * @brief     Reads the rest of iss, parses and executes command
  * @param     iss - Stream from which the commands are parsed
  * @details   Reads from iss and transposes variable, if the syntax was respected.
  */
  void ParseTranspose(std::istringstream& iss);

  /**
  * @fn        ParsePrint
  * @brief     Reads the rest of iss, parses and executes command
  * @param     iss - Stream from which the commands are parsed
  * @details   Reads from iss and prints variable, if the syntax was respected.
  */
  void ParsePrint(std::istringstream& iss) const;

  /**
  * @fn        ParseRank
  * @brief     Reads the rest of iss, parses and executes command
  * @param     iss - Stream from which the commands are parsed
  * @details   Reads from iss and prints the rank of variable, if the syntax was respected.
  */
  void ParseRank(std::istringstream& iss) const;

  /**
  * @fn        ParseSplit
  * @brief     Reads the rest of iss, parses and executes command
  * @param     iss - Stream from which the commands are parsed
  * @param     saveTo - Variable name, into which the result is to be saved
  * @details   Executes the command, if the command's syntax was respected
  * @details   and prints the result to os or saves it to calc, if saveTo isn't empty.
  */
  void ParseSplit(std::istringstream& iss, const std::string& saveTo);

  /**
  * @fn        ParseMerge
  * @brief     Reads the rest of iss, parses and executes command
  * @param     iss - Stream from which the commands are parsed
  * @param     saveTo - Variable name, into which the result is to be saved
  * @details   Executes the command, if the command's syntax was respected
  * @details   and prints the result to os or saves it to calc, if saveTo isn't empty.
  */
  void ParseMerge(std::istringstream& iss, const std::string& saveTo);

  /**
  * @fn        ParseDeterminant
  * @brief     Reads the rest of iss, parses and executes command
  * @param     iss - Stream from which the commands are parsed
  * @details   Reads from iss and prints the determinant of variable, if the syntax was respected.
  */
  void ParseDeterminant(std::istringstream& iss) const;

  void ParseInverse(std::istringstream& iss, const std::string& saveTo);

  /**
  * @fn        ParseAddMulSub
  * @brief     Reads the rest of iss, parses and executes command
  * @param     iss - Stream from which the commands are parsed
  * @param     saveTo - Variable name, into which the result is to be saved
  * @param     variable - First operand already read in Parse()
  * @details   Reads from iss an operator and an operand and executes given command over the two operands.
  */
  void ParseAddMulSub(std::istringstream& iss, const std::string& variable, const std::string& saveTo);

  /**
  * @fn        Scan
  * @brief     Reads the values from is width*height times and saves them to Matrix
  * @param     name - Name of the matrix variable to be created
  * @param     width - Number of collums in the matrix
  * @param     height - Number of rows in the matrix
  * @details   Depending on number of zeroes, it saves the values in Dense/SparseMatrix
  */
  void Scan(const std::string& name, int width, int height);

  /**
  * @fn        ReadArgument
  * @brief     Reads operation argument from iss
  * @param     iss - Stream from which the commands are parsed
  * @details   Reads the argument in "-X " format, where X is single char.
  * @returns   char - If there was an argument to be read in iss
  * @returns   0 - If there wasn't any argument in iss
  * @returns   1 - If there was an argument, but with incorrect format
  */
  char ReadArgument(std::istringstream& iss) const;

  /**
  * @fn        ToLower
  * @brief     Converts string's characters to lower characters
  * @param     text - Text to be converted
  * @returns   Converted string
  */
  std::string ToLower(const std::string& text) const;

  /**
  * @fn        GetRidOfSpaces
  * @brief     Ignores all space-like characters
  * @param     iss - Stream on which the command will be executed.
  */
  void GetRidOfSpaces(std::istringstream& iss) const;

  /**
  * @fn        ReadAlpha
  * @brief     Reads from iss until any non-alphabetic character is reached
  * @param     iss - Stream from which the string is extracted
  * @returns   String of alphabetic characters
  */
  std::string ReadAlpha(std::istringstream& iss) const;

  /**
  * @fn        ReadNum
  * @brief     Reads positive integer from iss
  * @param     iss - Stream from which the number is extracted
  * @returns   Extracted integer or -1 if there wan't any number to be read from iss
  */
  int ReadNum(std::istringstream& iss) const;

  /**
  * @fn        CheckAndGetChar
  * @brief     Checks the following character
  * @param     iss - Stream from which the char is to be extracted
  * @param     c - Char to which following char in iss will be compared
  * @details   Checks the following character in iss, if it's c, it extracts it
  * @returns   True  - If the character in iss was c and it was extracted
  * @returns   False - If the character in iss wasn't c and iss was left untouched
  */
  bool CheckAndGetChar(std::istringstream& iss, char c) const;

  /**
  * @fn        EndOfCommand
  * @brief     Checks if there are any non-white-space characters left in iss
  * @param     iss - Stream to be checked
  * @returns   True  - If there aren't any non-white-space characters left
  * @returns   False - If there are any non-white-space characters left
  */
  bool EndOfCommand(std::istringstream& iss) const;

  /**
  * @fn        CheckVariableUsage
  * @brief     Checks if a matrix of given name is in calc
  * @param     variable - Name of the variable to be checked
  */
  bool CheckVariableUsage(const std::string& variable) const;

  /**
  * @fn        WriteError
  * @brief     Writes out an error message
  * @param     msg - Error to be written
  * @details   Highlights the msg in bold red, passes it to os with endl at the end
  */
  void WriteError(const char * msg) const;

public:
  Parser(std::istream& is = std::cin, std::ostream& os = std::cout);

  /**
  * @fn        WriteError
  * @brief     Reads input from is while it can
  */
  void Run();
};


#endif
