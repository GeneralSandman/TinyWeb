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

#include "eventloop.h"
#include "netaddress.h"
#include "connector.h"
#include "socket.h"
#include "api.h"
#include "log.h"

Connector::Connector(EventLoop *loop,
                     const NetAddress &server,
                     bool retry,
                     int hostport)
    : m_pEventLoop(loop),
      m_nServerAddress(server),
      m_nRetry(retry),
      m_nConnectSocket(createNoBlockSocket()),
      m_nConnectChannel(loop,m_nConnectSocket.getFd())
{
    if (hostport != 0)
    {
        m_nConnectSocket.bindAddress(NetAddress(hostport));
    }
    //we have to close this socket fd.
    LOG(Debug) << "class Connector constructor\n";
}

Connector::~Connector()
{
    LOG(Debug) << "class Connector destructor\n";
}