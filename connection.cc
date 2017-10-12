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

#include "connection.h"
#include "netaddress.h"
#include "eventloop.h"
#include "channel.h"
#include "time.h"
#include "api.h"
#include "log.h"

#include <boost/bind.hpp>
#include <iostream>

void Connection::m_fHandleRead(Time arrive)
{
    ssize_t n = m_nInputBuffer.put(m_pChannel->getFd());

    if (n > 0)
    {
        if (m_nMessageCallback)
            m_nMessageCallback(this,
                               &m_nInputBuffer,
                               arrive);
    }
    else if (n == 0)
    {
        m_fHandleClose();
    }
    else
    {
        m_fHandleError();
    }
}

void Connection::m_fHandleWrite()
{
}

void Connection::m_fHandleClose()
{
    m_pChannel->disableAll();
    if (m_nCloseCallback)
        m_nCloseCallback(this);
}

void Connection::m_fHandleError()
{
    std::cout << "socket error\n";
}

Connection::Connection(EventLoop *loop,
                       int connectfd,
                       const NetAddress &local,
                       const NetAddress &peer)
    : m_pEventLoop(loop),
      m_nState(Connecting),
      m_pChannel(new Channel(loop, connectfd)),
      m_nLocalAddress(local),
      m_nPeerAddress(peer)
{
    m_pChannel->setReadCallback(boost::bind(&Connection::m_fHandleRead, this, _1));
    m_pChannel->setWriteCallback(boost::bind(&Connection::m_fHandleWrite, this));
    m_pChannel->setCloseCallback(boost::bind(&Connection::m_fHandleClose, this));
    m_pChannel->setErrorCallback(boost::bind(&Connection::m_fHandleError, this));

    LOG(Debug) << "class Connection constructor\n";
}

void Connection::establishConnection()
{
    m_nState = Connected;
    m_pChannel->enableRead();
    m_nConnectCallback(this);
}

void Connection::destoryConnection()
{
    m_nState = DisConnected;
    m_pChannel->disableAll();
    m_pEventLoop->removeChannel(m_pChannel);
    delete m_pChannel;
    m_pChannel = nullptr;
}

Connection::~Connection()
{
    LOG(Debug) << "class Connection destructor\n";
}
