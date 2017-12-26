/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*---class Connector---
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

typedef boost::function<void(int, const NetAddress &)> newConnectionCallback;

class Connector
{
private:
  EventLoop *m_pEventLoop;
  Socket m_nConnectSocket;
  Channel m_nConnectChannel;
  newConnectionCallback m_nCallback;
  bool m_nRetry;
  int m_nRetryTime;
  bool m_nConnected;

  void m_fHandleRead();

  void m_fEstablishConnection

public:
  Connector(EventLoop *, const NetAddress &, bool, int hostport = 0);
  void setConnectionCallback(newConnectionCallback c) { m_nCallback = c; }
  void connect();
  void retry();
  ~Connector();
};

#endif // !CONNECTOR_T