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

#ifndef SERVER_H
#define SERVER_H

#include "channel.h"
#include "accepter.h"
#include "callback.h"
#include "netaddress.h"

#include <boost/function.hpp>
#include <vector>
#include <map>
#include <set>

class EventLoop;
class Connection;
class Protocol;

class Server
{
private:
  bool m_nStarted;
  int m_nConNum;
  NetAddress m_nListenAddress;
  EventLoop *m_pEventLoop;
  Accepter m_nAccepter;
  std::set<Connection *> m_nConnections;
  ConnectionCallback m_nConnectCallback;
  MessageCallback m_nMessageCallback;
  CloseCallback m_nCloseCallback;

  Protocol *m_pProtocol;

  void m_fHandleRead(int, const NetAddress &);
  void m_fHandleClose(Connection *);

public:
  Server(EventLoop *, const NetAddress &, Protocol *);
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
  void start();
  ~Server();
};

#endif // !SERVER_H
