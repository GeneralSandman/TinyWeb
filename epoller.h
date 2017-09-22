#ifndef EPOLLER_H
#define EPOLLER_H

#include <vector>
#include <map>

class Channel;

class EPoller
{
private:
  int m_nEFd;
  int m_nEventNum;//in poll(),it will be seted by epoll_wait;
                  //and cleared after invoke m_fFillActiveChannels(),
  std::vector<struct epoll_event> m_nEvents;
  std::map<int, Channel *> m_nChannels;
  void m_fFillActiveChannels(std::vector<Channel *> &);
  void m_fUpdate(int, Channel *);

public:
  EPoller();
  void updateChannel(Channel *);
  void poll(std::vector<Channel *> &); //epoll_wait to push_back the active channels
  ~EPoller();
};

#endif