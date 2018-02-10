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

#include <tiny_core/slave.h>
#include <tiny_core/protocol.h>
#include <tiny_core/factory.h>
#include <tiny_core/eventloop.h>
#include <tiny_core/server.h>
#include <tiny_core/netaddress.h>

Slave::Slave(EventLoop *loop, int num, const std::string &name)
    : m_pEventLoop(loop),
      m_nNumber(num),
      m_nName(name),
      m_pProtocol(new WebProtocol()),
      m_pFactory(new Factory(m_pEventLoop, m_pProtocol)),
      m_nListenAddress(NetAddress("127.0.0.1:80"))
{
    LOG(Debug) << "class Slave constuctor\n";
}

void Slave::createListenServer(int listenSocket)
{
    m_nListenSocketFd = listenSocket;
    m_pServer = new Server(m_pEventLoop,
                           m_nListenAddress, listenSocket, m_pFactory);
}

void Slave::work()
{
    m_pServer->start();
    m_pEventLoop->loop();
}

Slave::~Slave()
{
    LOG(Debug) << "class Slave destructor\n";
}