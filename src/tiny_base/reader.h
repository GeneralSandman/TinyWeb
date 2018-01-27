/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*---Reader Class---
*Reader is a class which is used to read file.
*This class doesn't use IO(buffer)-class.
*We need to upgrade this class.
*Before we use readLine(),make sure invoke resetToHead()
****************************************
*readLine():
*   return current line content
*getCurrLineNu():
*   return current line number
*haveMore():
*   if file have more content to be readed,return true
*resetToHead():
*   reread begin the file head.
*/

#ifndef READER_H
#define READER_H

#include <iostream>
#include <fstream>
#include <string>

class Reader
{
private:
  std::string m_nFile;
  std::ifstream m_nFileStream;
  int m_nLine;
  int m_nCurrLine;

  void m_fInit(void);
  void m_fCountLine(void);

public:
  Reader(const std::string &f);
  void setFile(const std::string &f);
  std::string readLine(void);
  int getCurrLineNu(void) { return m_nCurrLine; }
  int getLineNu(void) { return m_nLine; }
  bool haveMore(void) { return m_nLine > m_nCurrLine; }
  void resetToHead(void);
  ~Reader();
};

#endif // !READER_H
