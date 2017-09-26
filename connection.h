#ifndef CONNECTION_H
#define CONNECTION_H

#include "channel.h"

#include <boost/function.hpp>

typedef boost::function<void()> connectCallback;
typedef boost::function<void()> messageCallback;

enum Connection_State
{
  Connecting,
  Connected
};

class EventLoop;  

class Connection
{
private:
  Connection_State m_nState;
  Channel m_nChannel; //connect fd
  connectCallback m_nConnectCallback;
  messageCallback m_nMessageCallback;

public:
  Connection(EventLoop *, int );
  void setConenctCallback(connectCallback c)
  {
    m_nConnectCallback = c;
  }
  void setMessageCallback(messageCallback c)
  {
    m_nMessageCallback = c;
  }
  ~Connection();
};

#endif // !CONNECTION_H