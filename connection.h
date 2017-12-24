/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*---class Connection---
*
****************************************
*
*/

#ifndef CONNECTION_H
#define CONNECTION_H

#include "channel.h"
#include "buffer.h"
#include "callback.h"
#include "time.h"
#include "netaddress.h"

#include <boost/function.hpp>

class Connection;
class Socket;
class EventLoop;

enum Connection_State
{
  Connecting,
  Connected,
  DisConnecting,
  DisConnected
};

class Connection
{
private:
  EventLoop *m_pEventLoop;
  Connection_State m_nState;
  Socket *m_pConnectSocket; //have difference between ListenSocket
  Channel *m_pChannel;      //connect fd
  ConnectionCallback m_nConnectCallback;
  MessageCallback m_nMessageCallback;
  CloseCallback m_nCloseCallback;

  Buffer m_nInputBuffer;
  Buffer m_nOutputBuffer;

  NetAddress m_nLocalAddress;
  NetAddress m_nPeerAddress;

  void m_fHandleRead(Time arrive);
  void m_fHandleWrite();
  void m_fHandleClose();
  void m_fHandleError();

  void m_fShutdownWrite();

public:
  Connection(EventLoop *, int, const NetAddress &, const NetAddress &);
  void send(const std::string &);
  void setConenctCallback(ConnectionCallback c)
  {
    m_nConnectCallback = c;
  }
  void setMessageCallback(MessageCallback c)
  {
    m_nMessageCallback = c;
  }
  void setCloseCallback(CloseCallback c)
  {
    m_nCloseCallback = c;
  }
  void shutdownWrite() { m_fShutdownWrite(); }
  void establishConnection();
  void destoryConnection();
  ~Connection();
};

#endif // !CONNECTION_H