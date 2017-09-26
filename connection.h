#ifndef CONNECTION_H
#define CONNECTION_H

#include "channel.h"
#include "netaddress.h"
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
  NetAddress m_nLocalAddress;
  NetAddress m_nPeerAddress;

  void m_fHeadleRead();

public:
  Connection(EventLoop *, int, const NetAddress &, const NetAddress &);
  void setConenctCallback(connectCallback c)
  {
    m_nConnectCallback = c;
  }
  void setMessageCallback(messageCallback c)
  {
    m_nMessageCallback = c;
  }

  void establishConnection();
  ~Connection();
};

#endif // !CONNECTION_H