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
#include "netaddress.h"

#include <boost/function.hpp>
#include <vector>
#include <map>
#include <set>

typedef boost::function<void()> connectCallback;
typedef boost::function<void()> messageCallback;
typedef boost::function<void()> closeCallback;

class EventLoop;
class Connection;

class Server
{
private:
  bool m_nStarted;
  int m_nConNum;
  NetAddress m_nListenAddress;
  EventLoop *m_pEventLoop;
  Accepter m_nAccepter;
  std::set<Connection *> m_nConnections;
  connectCallback m_nConnectCallback;
  messageCallback m_nMessageCallback;
  closeCallback m_nCloseCallback;

  void m_fHandleRead(int, const NetAddress &);
  void m_fHandleClose(Connection *);

public:
  Server(EventLoop *, const NetAddress &);
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
  void start();
  ~Server();
};

#endif // !SERVER_H
