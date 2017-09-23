#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H

#include "time.h"
#include "timer.h"

#include <vector>

class EPoller;
class Channel;
class TimerQueue;

class EventLoop
{
private:
  bool m_nRunning;
  EPoller *m_pPoller;
  std::vector<Channel *> m_nActiveChannels;
  TimerQueue *m_pTimerQueue;

public:
  EventLoop();
  void updateChannel(Channel *);
  void loop();
  void quit() { m_nRunning = false; }

  void runAt(Time, timerReadCallback);
  void runAfter(double, timerReadCallback);
  void runEvery(double, timerReadCallback);

  ~EventLoop();
};

#endif