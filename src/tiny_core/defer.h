/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*---XXX---
*
****************************************
*
*/

#ifndef DEFER_H
#define DEFER_H

#include <tiny_base/exception.h>
#include <tiny_base/log.h>

#include <deque>
#include <string>
#include <boost/function.hpp>

typedef boost::function<void()> callBack;
typedef boost::function<void(Exception &)> errorBack;

typedef std::pair<callBack, char **> callBackPair;
typedef std::pair<errorBack, char **> errorBackPair;

typedef std::pair<callBack, errorBack> Chain;

void defaultFun();
void defauleFunExce(Exception &e);

class Defer
{
private:
  std::deque<Chain> m_nChains;
  bool m_nNextChain;
  Exception m_nPreException;

  bool m_nIsCalling;
  bool m_nCalled;
  void m_fRunCallBacks();

public:
  Defer();
  void addCallBack(callBack c);
  void addErrorBack(errorBack e);
  void addCallBacks(callBack c, errorBack e);
  void callback();
  void errorback(Exception &);
  ~Defer();
};

#endif // !DEFER_H
