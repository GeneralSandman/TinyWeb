/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*---Client class---
*This class have same function like Server
****************************************
*
*/

#ifndef CLIENT_T
#define CLIENT_T

#include "netaddress.h"
#include "callback.h"

#include <set>

class EventLoop;
class Connector;
class Connection;
class Factory;

class Client
{
private:
  bool m_nStarted;
  unsigned long long m_nConNum;
  //host address.
  EventLoop *m_pEventLoop;
  std::set<Connector *> m_nConnectors;
  std::set<Connection *> m_nConnections;
  // std::set<std::pair<Connector *, Connection *>> m_nConnections_;
  ConnectionCallback m_nConnectCallback;
  MessageCallback m_nMessageCallback;
  WriteCompleteCallback m_nWriteCompleteCallback;
  CloseCallback m_nCloseCallback;
  Factory *m_pFactory;

public:
  Client(EventLoop *, Factory *);
  void setConenctCallback(ConnectionCallback c)
  {
    m_nConnectCallback = c;
  }
  void setMessageCallback(MessageCallback c)
  {
    m_nMessageCallback = c;
  }
  void setWriteCompleteCallback(WriteCompleteCallback c)
  {
    m_nWriteCompleteCallback = c;
  }
  void setCloseCallback(CloseCallback c)
  {
    m_nCloseCallback = c;
  }
  void start();
  void connect(const NetAddress &peeraddress, bool retry, int hostport = 0);
  ~Client();
};

#endif // !CLIENT_T