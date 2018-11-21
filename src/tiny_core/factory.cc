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

#include <tiny_core/factory.h>
#include <tiny_core/eventloop.h>
#include <tiny_core/protocol.h>
#include <tiny_core/timerid.h>
#include <tiny_base/buffer.h>
#include <tiny_base/log.h>

/*------------Factory-------------*/

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
    //invoke the member function of Protocol
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
        Protocol *prot = p->second;
        //invoke the member function of Protocol
        prot->getMessage(all);
    }
}

void Factory::writeComplete(Connection *con)
{
    //It's used by Server
    //can't be override
    auto p = m_nProtocols.find(con);
    if (p != m_nProtocols.end())
    {
        //invoke the member function of Protocol
        p->second->writeComplete();
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
        //invoke the member function of Protocol
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
        //invoke the member function of Protocol
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
    for (auto t : m_nProtocols)
    {
        if (t.second != nullptr)
        {
            delete t.second;
        }
    }
    LOG(Debug) << "class Factory destructor\n";
}

/*------------Factory-------------*/

/*------------ServerFactory-------------*/

ServerFactory::ServerFactory(EventLoop *loop, Protocol *protocol)
    : Factory(loop, protocol)
{
    LOG(Debug) << "class ServerFactory constructor\n";
}

void ServerFactory::buildProtocol(Protocol *newProt)
{
    //set member of Protocol.
}

ServerFactory::~ServerFactory()
{
    LOG(Debug) << "class ServerFactory destructor\n";
}

/*------------ServerFactory-------------*/

/*------------ClientFactory-------------*/

ClientFactory::ClientFactory(EventLoop *loop, Protocol *protocol)
    : Factory(loop, protocol)
{
    LOG(Debug) << "class ClientFactory constructor\n";
}

void ClientFactory::buildProtocol(Protocol *newProt)
{
    //set member of Protocol.
}

ClientFactory::~ClientFactory()
{
    LOG(Debug) << "class ClientFactory destructor\n";
}

/*-----------ClientFactory--------------*/