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

#include "factory.h"
#include "buffer.h"
#include "eventloop.h"
#include "protocol.h"
#include "log.h"

//---Factory api--------------//
Factory::Factory(EventLoop *loop, Protocol *prot)
    : m_pLoop(loop),
      m_nNumProts(0),
      m_pProtocol(prot)
{
    LOG(Debug) << "class Factory constructor\n";
}

void Factory::createConnection(Connection *newCon)
{
    //It's used by Server
    //can't be override
    Protocol *newProt = getInstanceByPointer(m_pProtocol); //FIXME:
    newProt->m_pFactory = this;
    newProt->m_nNumber = (++m_nNumProts);
    newProt->m_pConnection = newCon;
    m_nProtocols[newCon] = newProt;
    newProt->makeConnection();
    buildProtocol(newProt);
}

void Factory::getMessage(Connection *con,
                         Buffer *input,
                         Time time)
{
    //It's used by Server
    //can't be override
    auto p = m_nProtocols.find(con);
    if (p != m_nProtocols.end())
    {
        std::string all = input->getAll();
        // LOG(Debug) << all << std::endl;
        Protocol *prot = p->second;
        prot->getMessage(all);
    }
}

void Factory::lostConnection(Connection *con)
{
    //It's used by Server
    //can't be override
    auto p = m_nProtocols.find(con);
    if (p != m_nProtocols.end())
    {
        Protocol *prot = p->second;
        prot->loseConnection();

        delete prot;
        m_nProtocols.erase(p);
    }
}

void Factory::closeProtocol(Protocol *protocol)
{
    auto p = m_nProtocols.find(protocol->m_pConnection);
    if (p != m_nProtocols.end())
    {
        Connection *con = p->first;
        Protocol *prot = p->second;
        
        con->shutdownWrite();
        prot->loseConnection();

        delete prot;
        m_nProtocols.erase(p);
    }
}

void Factory::closeProtocolAfter(Protocol *protocol, int seconds)
{
    m_pLoop->runAfter(seconds, boost::bind(&Factory::closeProtocol, this, protocol));
}

void Factory::buildProtocol(Protocol *newProt)
{
    // can be override
}

Factory::~Factory()
{
    LOG(Debug) << "class Factory destructor\n";
}

//---ServerFactory api--------------//
// ServerFactory::ServerFactory()
// {
//     LOG(Debug) << "class ServerFactory constructor\n";
// }

// ServerFactory::~ServerFactory()
// {
//     LOG(Debug) << "class ServerFactory destructor\n";
// }

//---EchoServerFactory api--------------//
// EchoServerFactory::EchoServerFactory()
// {
//     LOG(Debug) << "class EchoServerFactory constructor\n";
// }

// EchoServerFactory::~EchoServerFactory()
// {
//     LOG(Debug) << "class EchoServerFactory destructor\n";
// }

//---WebServerFactory api--------------//
// WebServerFactory::WebServerFactory()
// {
//     LOG(Debug) << "class WebServerFactory constructor\n";
// }

// WebServerFactory::~WebServerFactory()
// {
//     LOG(Debug) << "class WebServerFactory destructor\n";
// }