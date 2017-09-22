#ifndef EPOLLER_H
#define EPOLLER_H

#include <vector>
#include <map>

class Channel;

class EPoller
{
  private:
    int m_nFd;
    std::vector<struct epoll_event> m_nEvents;
    std::map<int, Channel *> m_nChannels;
    //channels
    //epoll_event
    void m_fFillActiveChannels(std::vector<Channel *> &);

  public:
    EPoller();
    void poll(std::vector<Channel*> &); //epoll_wait to push_back the active channels
    ~EPoller();
};

#endif