/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*---class Accepter---
*This class is used by class Server to accept socket.
****************************************
*
*
*/

#ifndef ACCEPTER_H
#define ACCEPTER_H

#include <tiny_core/socket.h>
#include <tiny_core/channel.h>

#include <boost/function.hpp>

class EventLoop;

typedef boost::function<void(int, const NetAddress &)> newConnectionCallback;

class Accepter
{
private:
  EventLoop *m_pEventLoop;
  Socket m_nListenSocket;
  Channel m_nListenChannel;
  newConnectionCallback m_nCallback;
  bool m_nListening;
  void m_fHandleRead();

public:
  Accepter(EventLoop *, const NetAddress &,int listenSocket);
  void setConnectionCallback(newConnectionCallback c) { m_nCallback = c; }
  void listen();
  ~Accepter();
};

#endif // !ACCEPTER_H
