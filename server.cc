#include "server.h"
#include "connection.h"
#include "log.h"
#include "api.h"

#include <boost/ptr_container/ptr_set.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <memory>

void Server::m_fHandleRead(int connectfd, const NetAddress &address)
{
    // NetAddress local(getLocalAddr());
    Connection *newCon =
        new Connection(m_pEventLoop, connectfd, m_nListenAddress, address);
    m_nConNum++;
    std::cout << m_nConNum << std::endl;
    newCon->setConenctCallback(m_nConnectCallback);
    newCon->setMessageCallback(m_nMessageCallback);
    newCon->setCloseCallback(boost::bind(&Server::m_fHandleClose, this, _1));
    m_nConnections.insert(newCon);
    newCon->establishConnection();
}

void Server::m_fHandleClose(Connection *con)
{
    if (m_nCloseCallback)
        m_nCloseCallback();
    // std::cout << "lib code:remove connection\n";
    con->destoryConnection();
    auto p = m_nConnections.find(con);
    delete (*p);
    m_nConnections.erase(p);
}

Server::Server(EventLoop *loop, const NetAddress &address)
    : m_nStarted(false),
      m_nConNum(0),
      m_nListenAddress(address),
      m_pEventLoop(loop),
      m_nAccepter(loop, address)
{
    m_nAccepter.setConnectionCallback(
        boost::bind(&Server::m_fHandleRead, this, _1, _2));
    LOG(Debug) << "class Server constructor\n";
}

void Server::start()
{
    if (!m_nStarted)
    {
        m_nAccepter.listen();
        m_nStarted = true;
    }
}

Server::~Server()
{
    LOG(Debug) << "class Server destructor\n";
}