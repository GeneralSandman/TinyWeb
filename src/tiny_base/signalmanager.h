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

#include <tiny_struct/string_t.h>

// typedef std::function<void(int)> SignalCallback;
typedef void (*SignalCallback)(int);

class Signal
{
private:
  int m_nNumber;
  String m_nName;
  String m_nMeaning;
  SignalCallback m_nCallback;

public:
  Signal(int, const String &, const String &, SignalCallback);
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
