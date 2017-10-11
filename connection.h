/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*---XXX---
*
****************************************
*
*/

#ifndef CONNECTION_H
#define CONNECTION_H

#include "channel.h"
#include "buffer.h"
#include "time.h"
#include "netaddress.h"

#include <boost/function.hpp>

class Connection;

typedef boost::function<void()> connectCallback;
typedef boost::function<void(Connection *, Buffer *, Time)>
    messageCallback;
typedef boost::function<void(Connection *)> closeCallback_;
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
  EventLoop *m_pEventLoop;
  Connection_State m_nState;
  Channel *m_pChannel; //connect fd
  connectCallback m_nConnectCallback;
  messageCallback m_nMessageCallback;
  closeCallback_ m_nCloseCallback;
  // errorCallback m_nErrorCallback;

  Buffer m_nInputBuffer;
  Buffer m_nOutputBuffer;

  NetAddress m_nLocalAddress;
  NetAddress m_nPeerAddress;

  void m_fHandleRead(Time arrive);
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
  void setCloseCallback(closeCallback_ c)
  {
    m_nCloseCallback = c;
  }

  void establishConnection();
  void destoryConnection();
  ~Connection();
};

#endif // !CONNECTION_H