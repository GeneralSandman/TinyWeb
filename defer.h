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

#include "log.h"

#include <deque>
#include <string>
#include <boost/function.hpp>

class Exception;

typedef boost::function<void()> callBack;
typedef boost::function<void(Exception &)> errorBack;

typedef std::pair<callBack, char **> callBackPair;
typedef std::pair<errorBack, char **> errorBackPair;

typedef std::pair<callBack, errorBack> Chain;

void defaultFun()
{
}

void defauleFunExce(Exception &e)
{
}

class Defer
{
  private:
    std::deque<Chain> m_nChains;

  public:
    Defer();
    void addCallBack(callBack c);
    void addErrorBack(errorBack e);
    void addCallBacks(callBack c, errorBack e);

    void runCallBacks(Exception &e)
    {
    }

    void callback();
    void errorback(const std::string &std);
    ~Defer();
};

#endif // !DEFER_H
