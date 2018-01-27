/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*---class SignalManager---
*
****************************************
*
*/

#ifndef SIGNAL_MANAGER_H
#define SIGNAL_MANAGER_H

#include <string>
#include <functional>

typedef std::function<void()> SignalCallback;

class Signal
{
  private:
    int m_nSignalNumber;
    std::string m_nName;
    std::string m_nMeaning;
    SignalCallback m_nSignalCallback;

  public:
    Signal(int, const std::string &, const std::string&, SignalCallback);
    ~Signal();
};

class SignalManager
{
  private:
  public:
    SignalManager();
    void addSignal();
    void updateSignal();
    void deleteSignal();
    ~SignalManager();
};

#endif // !SIGNAL_MANAGER_H
