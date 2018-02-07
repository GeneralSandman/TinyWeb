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

// #include <tiny_struct/string_t.h>
#include <string>

// typedef std::function<void(int)> SignalCallback;
typedef void (*SignalCallback)(int);

class Signal
{
private:
  int m_nNumber;
  std::string m_nName;
  std::string m_nMeaning;
  SignalCallback m_nCallback;

public:
  Signal(int, const std::string &, const std::string &, SignalCallback);
  bool isSame(int signal) const
  {
    return m_nNumber == signal;
  }
  ~Signal();
  friend class SignalManager;
};

class SignalManager
{
private:
  unsigned char m_nNum;

public:
  SignalManager();
  void addSignal(const Signal &);
  void updateSignal(const Signal &);
  void deleteSignal(const Signal &);
  ~SignalManager();
};

#endif // !SIGNAL_MANAGER_H
