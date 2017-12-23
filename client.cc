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

#include "client.h"
#include "connector.h"
#include "log.h"

Client::Client(EventLoop *loop,
               Factory *factory)
    : m_nStarted(false),
      m_nConNum(0),
      m_pEventLoop(loop),
      m_pFactory(factory)
{
    LOG(Debug) << "class Client constructor\n";
}

void Client::start()
{
    m_nStarted = true;
}

void Client::connect(const NetAddress &peeraddress, bool retry, int hostport)
{
    Connector *newConnector = new Connector(m_pEventLoop,
                                            peeraddress,
                                            retry,
                                            hostport);

    m_nConnectors.insert(newConnector);

    newConnector->connect();
}

Client::~Client()
{
    for (auto t : m_nConnectors)
    {
        delete t;
    }
    for (auto t : m_nConnections)
    {
        // delete t;
    }

    LOG(Debug) << "class Client destructor\n";
}