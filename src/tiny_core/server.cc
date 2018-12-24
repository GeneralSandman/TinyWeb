/*
 *Author:GeneralSandman
 *Code:https://github.com/GeneralSandman/TinyWeb
 *E-mail:generalsandman@163.com
 *Web:www.dissigil.cn
 */

/*---XXX---
 *
 ****************************************
 *
 */

#include <tiny_base/api.h>
#include <tiny_base/log.h>
#include <tiny_core/connection.h>
#include <tiny_core/factory.h>
#include <tiny_core/server.h>

#include <boost/bind.hpp>
#include <boost/ptr_container/ptr_set.hpp>
#include <iostream>
#include <memory>

void Server::m_fHandleRead(int connectfd, const NetAddress& address)
{
    // NetAddress local(getLocalAddr());
    Connection* newCon = new Connection(m_pEventLoop,
        connectfd,
        m_nListenAddress,
        address);
    m_nConNum++;
    // std::cout << m_nConNum << std::endl;
    newCon->setConnectCallback(m_nConnectCallback);
    newCon->setMessageCallback(m_nMessageCallback);
    newCon->setWriteCompleteCallback(m_nWriteCompleteCallback);
    newCon->setCloseCallback(boost::bind(&Server::m_fHandleClose, this, _1));
    m_nConnections.insert(newCon);
    newCon->establishConnection();
}

void Server::m_fHandleClose(Connection* con)
{
    if (m_nCloseCallback)
        m_nCloseCallback(con);
    // std::cout << "lib code:remove connection\n";
    con->destoryConnection();
    auto p = m_nConnections.find(con);
    delete (*p);
    m_nConnections.erase(p);
}

Server::Server(EventLoop* loop, const NetAddress& address,
    int listenSocket, Factory* fact)
    : m_nStarted(false)
    , m_nConNum(0)
    , m_nListenAddress(address)
    , m_pEventLoop(loop)
    , m_pFactory(fact)
    , m_nAccepter(loop, address, listenSocket)
{
    m_nAccepter.setConnectionCallback(
        boost::bind(&Server::m_fHandleRead, this, _1, _2));
    if (m_pFactory != nullptr) {
        setConnectCallback(m_pFactory->connectCallback());
        setMessageCallback(m_pFactory->getMessageCallback());
        setWriteCompleteCallback(m_pFactory->writeCompleteCallback());
        setCloseCallback(m_pFactory->closeConnectionCallback());
    }

    LOG(Debug) << "class Server constructor\n";
}

void Server::start()
{
    if (!m_nStarted) {
        m_nAccepter.listen();
        m_nStarted = true;
    }
}

Server::~Server()
{
    LOG(Info) << "rest connection number:" << m_nConnections.size() << std::endl;

    //have connection not done
    for (auto t : m_nConnections) {
        t->shutdownWrite();
        t->destoryConnection();
        delete t;
        //Think carefully.
    }
    LOG(Debug) << "class Server destructor\n";
}
