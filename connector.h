/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*---Connector class---
*This class has same function like Accepter.
*It is used by client to connect server.
*It will handler more errors.
* reconnect server
* server refuse to connect
* 
****************************************
*
*/

#ifndef CONNECTOR_T
#define CONNECTOR_T

#include "netaddress.h"
#include "socket.h"
#include "channel.h"

class EventLoop;

class Connector
{
private:
  EventLoop *m_pEventLoop;
  NetAddress m_nServerAddress;
  bool  m_nRetry;
  Socket m_nConnectSocket;
  Channel m_nConnectChannel;

public:
  Connector(EventLoop *, const NetAddress &,bool ,int hostport=0);
  void connect();
  void retry();
  ~Connector();
};

#endif // !CONNECTOR_T