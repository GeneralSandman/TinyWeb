/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*---class Exception---
*
****************************************
*
*/

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <string>

class Exception
{
private:
  std::string m_nName;
  std::string m_nStack;
  void m_fFillStackTrace();

public:
  explicit Exception(const char *msg);
  explicit Exception(const std::string &msg);
  Exception(const Exception &);
  const std::string &what() throw() { return m_nName; }
  // const char *what()throw(){return m_nName.c_str();};
  const std::string &stackTrace() throw() { return m_nStack; }
  //const char* stackTrace()throw(){return m_nStack.c_str();}
  virtual ~Exception() throw() {}
};

#endif