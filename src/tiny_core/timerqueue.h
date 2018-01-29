/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*---class TimerQueue---
*This class is used to manage class Timer.
*If we get read event from timefd,we should
*to find timers which are expires.
*Timers we have added to TimerQueue multiplex
*one timer file description.
*addTimer()
*   add a timer to queue,used by eventLoop
*cancelTimer()
*   cancel a timer by a TimerId,
*   TimerId can getted by addTimer.
****************************************
*
*/

#ifndef TIMER_QUEUE_H
#define TIMER_QUEUE_H

#include <tiny_core/timer.h>
#include <tiny_core/timerid.h>

#include <vector>
#include <set>

struct timespec howMuchTimeFromNow(Time when);

class Channel;
class EventLoop;

class TimerQueue
{
private:
  static unsigned long long m_nCreatedTimers; //++
  std::set<std::pair<Time, Timer *>> m_nTimers;
  std::set<std::pair<Timer *, unsigned long long>> m_nActiveTimers; //++
  //must m_nTimers.size()==m_nActiveTimers.size()
  bool m_nIsCallingExpiredTimers;
  std::set<std::pair<Timer *, unsigned long long>> m_nCancelingTimers; //++

  EventLoop *m_pEventLoop;
  int m_nFd;
  Channel *m_pTimeChannel;
  void m_fHandleRead();
  void m_fInsertTimer(Timer *timer);
  void m_fGetHappen(std::vector<Timer *> &happened);
  void m_fResetHappened(std::vector<Timer *> &happened);
  void m_fResetTimeFd(Time expiration);

public:
  TimerQueue(EventLoop *loop);
  //getNextTimerId() only can be used by Timer.
  static unsigned long long getNextTimerId() { return m_nCreatedTimers++; }       //++
  TimerId addTimer(Time &time, timerReadCallback c, bool repet, double interval); //create Timer
  void cancelTimer(TimerId &timerid);
  ~TimerQueue();
};

#endif