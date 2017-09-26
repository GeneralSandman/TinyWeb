#include "connection.h"
#include "netaddress.h"
#include "channel.h"
#include "api.h"
#include "log.h"

#include <boost/bind.hpp>
#include <iostream>

void Connection::m_fHandleRead()
{

    char buf[65536];
    ssize_t n = ::read(m_nChannel.getFd(), buf, sizeof buf);
    std::cout << buf << std::endl;

    if (n > 0)
    {
        if (m_nMessageCallback)
            m_nMessageCallback();
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
    m_nChannel.disableAll();
    if (m_nCloseCallback)
        m_nCloseCallback();
}

void Connection::m_fHandleError()
{
    std::cout << "socket error\n";
}

Connection::Connection(EventLoop *loop, int connectfd,
                       const NetAddress &local, const NetAddress &peer)
    : m_nState(Connecting),
      m_nChannel(loop, connectfd),
      m_nLocalAddress(local),
      m_nPeerAddress(peer)
{
    m_nChannel.setReadCallback(boost::bind(&Connection::m_fHandleRead, this));
    LOG(Debug) << "class Connection constructor\n";
}

void Connection::establishConnection()
{
    m_nState = Connected;
    m_nChannel.enableRead();
    m_nConnectCallback();
}

void Connection::destoryConnection()
{
    m_nState = DisConnected;
}

Connection::~Connection()
{
    LOG(Debug) << "class Connection destructor\n";
}
