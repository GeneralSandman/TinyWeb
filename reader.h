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

  void m_fCountLine(void);

public:
  Reader(const std::string &f);
  std::string readLine(void);
  int getCurrLineNu(void) { return m_nCurrLine; }
  int getLineNu(void) { return m_nLine; }
  bool haveMore(void) { return m_nLine > m_nCurrLine; }
  void resetToHead(void);
  ~Reader();
};

#endif // !READER_H
