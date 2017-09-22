#ifndef CHANNEL_H
#define CHANNEL_H

#include <sys/epoll.h>
#include <boost/function.hpp>

class EventLoop;

static int ChannelFlag_New = -1;
static int ChannelFlag_Added = -2;
static int ChannelFlag_Deleted = -3;

typedef boost::function<void()> callback;

class Channel
{
private:
  EventLoop *m_pEventLoop;
  int m_nFd;
  callback m_fReadCallback;
  //
  //
  //
  int m_nEvent;
  int m_nREvent;
  int m_nFlag; //used by EPoller::m_nEvents;
  void m_fUpdate();

public:
  Channel(EventLoop *, int fd);

  bool isNoneEvent() { return 0 == m_nEvent; }
  int getFd() { return m_nFd; }
  int getFlag() { return m_nFlag; }
  void setFlag(int f) { m_nFlag = f; }

  int getEvent() { return m_nEvent; }
  void handleEvent();

  void setREvent(int event) { m_nREvent = event; }

  void setReadCallback(const callback &c) { m_fReadCallback = c; };
  // void setWriteCallback();
  // void setCloseCallback();
  // void setErrorCallback();

  void enableRead()
  {
    m_nEvent |= EPOLLIN;
    m_fUpdate();
  }
  // void enableWrite();
  // void enableAll();

  void disableRead()
  {
    m_nEvent &= ~EPOLLIN;
    m_fUpdate();
  }
  // void disableWrite();
  // void disableAll();

  ~Channel();
};

#endif