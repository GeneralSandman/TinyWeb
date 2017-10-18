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
#include "protocol.h"
#include "log.h"

//---Factory api--------------//
Factory::Factory(const Protocol &prot)
    : m_nNumProts(0),
      m_nProtocol(prot)
{
    LOG(Debug) << "class Factory constructor\n";
}

void Factory::createConnection(Connection *newCon)
{
    //It's used by Server
    //can't be override
    Protocol *newProt = new Protocol();
    newProt->m_pConnection = newCon;
    newProt->m_nNumber = (++m_nNumProts);
    newProt->m_pFactory = this;
    m_nProtocols[newCon] = newProt;
    newProt->makeConnection();
}

void Factory::getMessage(Connection *con,
                         Buffer *buf,
                         Time time)
{
    //It's used by Server
    //can't be override
    auto p = m_nProtocols.find(con);
    if (p != m_nProtocols.end())
    {
        Protocol *prot = p->second;
        prot->getMessage();
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

Protocol *Factory::buildProtocol()
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