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

#include <tiny_core/accepter.h>
#include <tiny_core/eventloop.h>
#include <tiny_core/channel.h>
#include <tiny_core/socket.h>
#include <tiny_core/netaddress.h>
#include <tiny_base/api.h>
#include <tiny_base/log.h>

#include <boost/bind.hpp>

void Accepter::m_fHandleRead()
{
    NetAddress newConnectionAddress;
    int connectfd = m_nListenSocket.accept(newConnectionAddress);
    if (connectfd > 0)
    {
        if (m_nCallback)
            m_nCallback(connectfd, newConnectionAddress);
    }
    else
    {
        //FIXME:
        //handle error event
    }
}

Accepter::Accepter(EventLoop *loop, const NetAddress &server,int listenSocket)
    : m_pEventLoop(loop),
      m_nListenSocket(listenSocket),
      m_nListenChannel(loop, m_nListenSocket.getFd()),
      m_nListening(false)
{
    m_nListenChannel.setReadCallback(
        boost::bind(&Accepter::m_fHandleRead, this));
    //m_nListenSocket.bindAddress(server);

    LOG(Debug) << "class Accepter constructor\n";
}

void Accepter::listen()
{
    m_nListenChannel.enableRead();
    m_nListenSocket.listen();
    m_nListening = true;
}

Accepter::~Accepter()
{
    LOG(Debug) << "class Accepter destructor\n";
}