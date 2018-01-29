/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*---class Timer---
*
****************************************
*
*/

#ifndef TIMER_H
#define TIMER_H

#include <tiny_core/time.h>

#include <boost/function.hpp>

typedef boost::function<void()> timerReadCallback;

class Timer
{
private:
  Time m_nTime;
  unsigned long long m_nIdNum; //++++
  bool m_nRepet;
  double m_nInterval;
  timerReadCallback m_nCallback;

public:
  Timer(Time &time, timerReadCallback c, bool repet, double interval);
  Time getTime() { return m_nTime; }
  unsigned long long getIdNum() { return m_nIdNum; }//++
  void run() { m_nCallback(); }
  bool isRepet() { return m_nRepet; }
  void reset();
  ~Timer();
};

#endif // !TIMER_H