#ifndef CONNECTION_H
#define CONNECTION_H

#include "channel.h"
#include "netaddress.h"
#include <boost/function.hpp>

typedef boost::function<void()> connectCallback;
typedef boost::function<void()> messageCallback;
typedef boost::function<void()> closeCallback;
typedef boost::function<void()> errorCallback;

enum Connection_State
{
  Connecting,
  Connected,
  DisConnecting,
  DisConnected
};

class EventLoop;

class Connection
{
private:
  Connection_State m_nState;
  Channel m_nChannel; //connect fd
  connectCallback m_nConnectCallback;
  messageCallback m_nMessageCallback;
  closeCallback m_nCloseCallback;
  errorCallback m_nErrorCallback;

  NetAddress m_nLocalAddress;
  NetAddress m_nPeerAddress;

  void m_fHandleRead();
  void m_fHandleWrite();
  void m_fHandleClose();
  void m_fHandleError();

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
  void setCloseCallback(closeCallback c)
  {
    m_nCloseCallback = c;
  }

  void establishConnection();
  void destoryConnection();
  ~Connection();
};

#endif // !CONNECTION_H