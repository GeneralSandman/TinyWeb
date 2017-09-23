#ifndef TIMER_QUEUE_H
#define TIMER_QUEUE_H

#include "timer.h"

#include <vector>
#include <set>

struct timespec howMuchTimeFromNow(Time when);

class Channel;
class EventLoop;

class TimerQueue
{
  private:
    std::set<std::pair<Time, Timer *>> m_nTimers;
    int m_nFd;
    Channel *m_pTimeChannel;
    void m_fHandleRead();
    void m_fInsertTimer(Timer *);
    void m_fGetHappen(std::vector<Timer *> &);
    void m_fResetHappened(std::vector<Timer *> &);
    void m_fResetTimeFd();

  public:
    TimerQueue(EventLoop *);
    void addTimer(Time &time, timerReadCallback c, bool repet, double interval); //create Timer
    ~TimerQueue();
};

#endif