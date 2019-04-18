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

#include <tiny_base/log.h>
#include <tiny_core/client.h>
#include <tiny_core/connection.h>
#include <tiny_core/connector.h>
#include <tiny_core/factory.h>

#include <boost/bind.hpp>

void Client::m_fNewConnectionCallback(int sockfd,
    const NetAddress& hostaddress,
    const NetAddress& peeraddress) //this arg may be error.
{
    // this function must identifer which localaddress
    // establish connection.
    Connection* newCon = new Connection(m_pEventLoop,
        sockfd,
        hostaddress,
        peeraddress);
    m_nConNum++;
    // std::cout << m_nConNum << std::endl;
    newCon->setConnectCallback(m_nConnectCallback);
    newCon->setMessageCallback(m_nMessageCallback);
    newCon->setWriteCompleteCallback(m_nWriteCompleteCallback);
    newCon->setCloseCallback(boost::bind(&Client::m_fHandleClose, this, _1));

    AddressCouple add(hostaddress, peeraddress);
    assert(m_nConnections.find(add) != m_nConnections.end());
    m_nConnections[add].second = newCon;
    newCon->establishConnection();
}

void Client::m_fHandleClose(Connection* con)
{
    LOG(Debug) << "client handle close callback\n";

    if (m_nCloseCallback)
        m_nCloseCallback(con);

    AddressCouple add(con->getLocalAddress(), con->getPeerAddress());
    auto p = m_nConnections.find(add);
    assert(p != m_nConnections.end());

    Connector* conr = p->second.first;
    Connection* conn = p->second.second;

    conr->stop();
    conn->destoryConnection();

    delete conn;
    if (!conr->isKeepConnect()) { //can't be reused.
        delete conr;
        m_nConnections.erase(p);
    } else { //This pair may be reused next time.
    }

    if (m_nStarted && conr->isKeepConnect())
        conr->restart();
}

Client::Client(EventLoop* loop,
    Factory* factory)
    : m_nStarted(false)
    , m_nConNum(0)
    , m_pEventLoop(loop)
    , m_pFactory(factory)
{
    if (m_pFactory != nullptr) {
        setConnectCallback(m_pFactory->connectCallback());
        setMessageCallback(m_pFactory->getMessageCallback());
        setWriteCompleteCallback(m_pFactory->writeCompleteCallback());
        setCloseCallback(m_pFactory->closeConnectionCallback());
    }

    LOG(Debug) << "class Client constructor\n";
}

void Client::start()
{
    m_nStarted = true;
}

void Client::connect(const NetAddress& hostAddress,
    const NetAddress& peeraddress,
    bool retry,
    bool keepconnect)
{
    assert(m_nStarted);

    Connector* newConnector = new Connector(m_pEventLoop,
        hostAddress,
        peeraddress,
        retry,
        keepconnect);
    newConnector->setConnectionCallback(boost::bind(&Client::m_fNewConnectionCallback,
        this, _1, _2, _3));
    AddressCouple address(hostAddress, peeraddress);
    ConnectorCouple couple(newConnector, nullptr);
    m_nConnections[address] = couple;

    newConnector->start();
}

void Client::disconnect(const NetAddress& peeraddress,
    const NetAddress& hostaddress)
{
    AddressCouple address(peeraddress, hostaddress);
    auto p = m_nConnections.find(address);
    if (p != m_nConnections.end()) {
        Connector* conr = p->second.first;
        Connection* conn = p->second.second;
        conr->stop();
        conn->shutdownWrite();
        //stop this connection by connector.
        //FIXME:
        delete conr;
        delete conn;
        m_nConnections.erase(p);
    } else {
        //this connection don't exist.
        std::cout << "connection[" << peeraddress.getIpPort() << "-"
                  << hostaddress.getIpPort() << "] don't exist" << std::endl;
    }
}

void Client::disconnectAll()
{
    for (auto t : m_nConnections) {
        Connector* conr = t.second.first;
        Connection* conn = t.second.second;

        if (nullptr != conr)
            conr->stop();
        if (nullptr != conn)
            conn->shutdownWrite();
        //close this connection
        //stop this connection by connector.
        //FIXME:
        delete conr;
        delete conn;
    }

    m_nConnections.clear();
}

void Client::stop()
{
    disconnectAll();
    m_nStarted = false;
}

Client::~Client()
{
    stop();

    LOG(Debug) << "class Client destructor\n";
}
