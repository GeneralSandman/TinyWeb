/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*---class Accepter---
*This class is used by class Server to accept socket.
****************************************
*void setConnectionCallback(newConnectionCallback c);
*void listen();//invoke system call listen()
*/

#ifndef ACCEPTER_H
#define ACCEPTER_H

#include "socket.h"
#include "channel.h"

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
    Accepter(EventLoop *, const NetAddress &);
    void setConnectionCallback(newConnectionCallback c) { m_nCallback = c; }
    void listen();
    ~Accepter();
};

#endif // !ACCEPTER_H
