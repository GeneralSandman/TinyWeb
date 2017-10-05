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

#include "accepter.h"
#include "eventloop.h"
#include "channel.h"
#include "socket.h"
#include "netaddress.h"
#include "api.h"
#include "log.h"

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
        //handle error event
    }
}

Accepter::Accepter(EventLoop *loop, const NetAddress &server)
    : m_pEventLoop(loop),
      m_nListenSocket(createNoBlockSocket()),
      m_nListenChannel(loop, m_nListenSocket.getFd()),
      m_nListening(false)
{
    m_nListenChannel.setReadCallback(
        boost::bind(&Accepter::m_fHandleRead, this));
    m_nListenSocket.bindAddress(server);

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