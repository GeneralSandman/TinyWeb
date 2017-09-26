#include "connection.h"
#include "netaddress.h"
#include "channel.h"
#include "api.h"
#include "log.h"

#include <boost/bind.hpp>
#include <iostream>

void Connection::m_fHeadleRead()
{

    char buf[65536];
    ssize_t n = ::read(m_nChannel.getFd(), buf, sizeof buf);
    std::cout << buf << std::endl;

    if (m_nMessageCallback)
    {
        m_nMessageCallback();
        writeString(m_nChannel.getFd(), "I Get you message");
        close(m_nChannel.getFd());
    }
}

Connection::Connection(EventLoop *loop, int connectfd,
                       const NetAddress &local, const NetAddress &peer)
    : m_nState(Connecting),
      m_nChannel(loop, connectfd),
      m_nLocalAddress(local),
      m_nPeerAddress(peer)
{
    m_nChannel.setReadCallback(boost::bind(&Connection::m_fHeadleRead, this));
    LOG(Debug) << "class Connection constructor\n";
}

void Connection::establishConnection()
{
    m_nState = Connected;
    m_nChannel.enableRead();
    m_nConnectCallback();
}

Connection::~Connection()
{
    LOG(Debug) << "class Connection destructor\n";
}
