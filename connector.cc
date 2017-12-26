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

void Connector::m_fHandleRead()
{
}

Connector::Connector(EventLoop *loop,
                     const NetAddress &server,
                     bool retry,
                     int hostport)
    : m_pEventLoop(loop),
      m_nConnectSocket(createNoBlockSocket()),
      m_nConnectChannel(loop, m_nConnectSocket.getFd())
          m_nRetry(retry),
      m_nRetryTime(0),
      m_nConnected(false)
{
    if (hostport != 0)
    {
        m_nConnectSocket.bindAddress(NetAddress(hostport));
    }
    //we have to close this socket fd.
    LOG(Debug) << "class Connector constructor\n";
}

void Connector::connect()
{
    int res = m_nConnectSocket.connect(m_nServerAddress);
    int savedErrno = (res == 0) ? 0 : errno;
    switch (savedErrno)
    {
    case 0:
        connecting(sockfd);
        break;

    case ECONNREFUSED:
        retry(sockfd);
        break;

    case EFAULT:
        sockets::close(sockfd);
        break;

    default:
        sockets::close(sockfd);
        // connectErrorCallback_();
        break;
    }
}

void Connector::retry()
{
}

Connector::~Connector()
{
    LOG(Debug) << "class Connector destructor\n";
}