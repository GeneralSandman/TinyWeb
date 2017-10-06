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
 
#ifndef TIMER_H
#define TIMER_H

#include "time.h"

#include <boost/function.hpp>

typedef boost::function<void()> timerReadCallback;

class Timer
{
  private:
    Time m_nTime;
    bool m_nRepet;
    double m_nInterval;
    timerReadCallback m_nCallback;

  public:
    Timer(Time &time, timerReadCallback c, bool repet, double interval);
    Time getTime() { return m_nTime; }
    void run() { m_nCallback(); }
    bool isRepet() { return m_nRepet; }
    void reset();
    ~Timer();
};

#endif // !TIMER_H