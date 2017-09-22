#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H

#include <vector>

class EPoller;
class Channel;

class EventLoop
{
private:
  bool m_nRunning;
  EPoller *m_pPoller;
  std::vector<Channel *> m_nActiveChannels;

public:
  EventLoop();
  void updateChannel(Channel *);
  void loop();
  void quit() { m_nRunning = false; }
  ~EventLoop();
};

#endif