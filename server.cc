#include "server.h"
#include "connection.h"
#include "log.h"
#include "api.h"

#include <boost/ptr_container/ptr_vector.hpp>
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
    m_nConnections.push_back(newCon);
    newCon->establishConnection();
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