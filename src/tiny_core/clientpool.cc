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
#include <tiny_core/clientpool.h>
#include <tiny_core/connection.h>
#include <tiny_core/connector.h>
#include <tiny_core/eventloop.h>
#include <tiny_core/factory.h>

#include <boost/bind.hpp>

void ClientPool::m_nConnectCallback(Connection* con)
{
    LOG(Info) << "invoke connect-callback of Protocol\n";

    auto p = m_nProtocols.find(con);
    // assert(p != m_nProtocols.end());
    if (p != m_nProtocols.end()) {
        Protocol* prot = p->second;
        prot->makeConnection();
    } else {
    }
}
void ClientPool::m_nMessageCallback(Connection* con,
    Buffer* input, Time time)
{
    LOG(Info) << "invoke message-callback of Protocol\n";

    auto p = m_nProtocols.find(con);
    assert(p != m_nProtocols.end());
    Protocol* prot = p->second;

    std::string all = input->getAll();
    prot->getMessage(all);
}
void ClientPool::m_nWriteCompleteCallback(Connection* con)
{
    LOG(Info) << "invoke writecomplete-callback of Protocol\n";

    auto p = m_nProtocols.find(con);
    assert(p != m_nProtocols.end());
    Protocol* prot = p->second;

    prot->writeComplete();
}
void ClientPool::m_nCloseCallback(Connection* con)
{
    LOG(Info) << "invoke close-callback of Protocol\n";

    auto p = m_nProtocols.find(con);
    assert(p != m_nProtocols.end());
    Protocol* prot = p->second;

    prot->loseConnection();

    m_nProtocols.erase(p);
}

void ClientPool::m_fNewConnectionCallback(int sockfd,
    const NetAddress& hostaddress,
    const NetAddress& peeraddress) //this arg may be error.
{
    //this function must identifer which localaddress
    //establish connection.
    Connection* newCon = new Connection(m_pEventLoop,
        sockfd,
        hostaddress,
        peeraddress);
    m_nConNum++;
    // std::cout << m_nConNum << std::endl;

    ConnectionCallback tmp_nConnectCallback = ConnectionCallback(
        boost::bind(
            &ClientPool::m_nConnectCallback,
            this, _1));
    MessageCallback tmp_nMessageCallback = MessageCallback(
        boost::bind(
            &ClientPool::m_nMessageCallback,
            this, _1, _2, _3));
    WriteCompleteCallback tmp_nWriteCompleteCallback = WriteCompleteCallback(
        boost::bind(
            &ClientPool::m_nWriteCompleteCallback, this, _1));
    CloseCallback tmp_nCloseCallback = CloseCallback(
        boost::bind(
            &ClientPool::m_fHandleClose,
            this, _1));

    newCon->setConnectCallback(tmp_nConnectCallback);
    newCon->setMessageCallback(tmp_nMessageCallback);
    newCon->setWriteCompleteCallback(tmp_nWriteCompleteCallback);
    newCon->setCloseCallback(tmp_nCloseCallback);

    AddressCouple add(hostaddress, peeraddress);
    assert(m_nConnections.find(add) != m_nConnections.end());
    // m_nConnections[add].second = newCon;
    assert(!m_nConnections[add].empty());
    ConnectorCouple couple = m_nConnections[add].front();
    m_nConnections[add].pop_front();
    assert(couple.second == nullptr);
    couple.second = newCon;
    m_nConnections[add].push_back(couple);
}

void ClientPool::m_fHandleClose(Connection* con)
{
    m_nCloseCallback(con);

    AddressCouple add(con->getLocalAddress(), con->getPeerAddress());
    auto p = m_nConnections.find(add);
    assert(p != m_nConnections.end());

    ConnectorCouple couple;
    for (auto i = m_nConnections[add].begin(); i != m_nConnections[add].end(); i++) {
        couple = *i;
        if (couple.second == con) {
            m_nConnections[add].erase(i);
            break;
        }
    }

    LOG(Debug) << m_nConnections.size() << std::endl;

    Connector* conr = couple.first;
    Connection* conn = couple.second;

    conr->stop();
    conn->destoryConnection();

    delete conn;
    if (!conr->isKeepConnect()) { //can't be reused.
        delete conr;
    } else { //This pair may be reused next time.
    }

    if (m_nStarted && conr->isKeepConnect())
        conr->restart();
}

ClientPool::ClientPool(EventLoop* loop,
    const NetAddress& hostAddress)
    : m_pEventLoop(loop)
    , m_nStarted(false)
    , m_nConNum(0)
    , m_nHostAddress(hostAddress)
    , m_nCapacity(0)
    , m_nUsing(0)
{
    LOG(Debug) << "class ClientPool constructor\n";
}

void ClientPool::closeProtocol(Protocol* protocol)
{
    // Don't use it now.
    auto p = m_nProtocols.find(protocol->m_pConnection);
    if (p != m_nProtocols.end()) {
        Connection* con = p->first;
        Protocol* prot = p->second;

        con->shutdownWrite();
        //invoke the member function of Protocol
        prot->loseConnection();

        delete prot;
        m_nProtocols.erase(p);
    }
}

void ClientPool::closeProtocolAfter(Protocol* protocol, int seconds)
{
    // Don't use it now.
    m_pEventLoop->runAfter(seconds, boost::bind(&ClientPool::closeProtocol, this, protocol));
}

void ClientPool::addClient(const NetAddress& hostAddress,
    const NetAddress& peerAddress,
    bool retry,
    bool keepConnect,
    int num)
{
}

void ClientPool::doTask(const NetAddress& hostAddress,
    const NetAddress& peerAddress,
    Protocol* protocol)
{
    AddressCouple address(hostAddress, peerAddress);
    auto p = m_nConnections.find(address);
    if (p == m_nConnections.end()) {
        std::cout << "no have connection, need to build new one\n";
        return;
    }
    ConnectorCouple conCouple = m_nConnections[address].front();

    Connection* con = conCouple.second;

    m_nProtocols[con] = protocol;
    // Init some number of Protocol;
    protocol->m_pFactory = nullptr;
    protocol->m_nNumber = 1;
    protocol->m_pConnection = con;

    con->establishConnection();
    // protocol->makeConnection();
    buildProtocol(protocol);
}

void ClientPool::start()
{
    m_nStarted = true;
}

void ClientPool::connect(const NetAddress& hostAddress,
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
    newConnector->setConnectionCallback(boost::bind(&ClientPool::m_fNewConnectionCallback,
        this, _1, _2, _3));

    AddressCouple address(hostAddress, peeraddress);
    ConnectorCouple couple(newConnector, nullptr);
    m_nConnections[address].push_back(couple);

    newConnector->start();
}

void ClientPool::disconnect(const NetAddress& peeraddress,
    const NetAddress& hostaddress)
{
    // AddressCouple address(peeraddress, hostaddress);
    // auto p = m_nConnections.find(address);
    // if (p != m_nConnections.end()) {
    //     Connector* conr = p->second.first;
    //     Connection* conn = p->second.second;
    //     conr->stop();
    //     conn->shutdownWrite();
    //     delete conr;
    //     delete conn;
    //     m_nConnections.erase(p);
    // } else {
    //     LOG(Info) << "connection[" << peeraddress.getIpPort() << "-"
    //               << hostaddress.getIpPort() << "] don't exist" << std::endl;
    // }
}

void ClientPool::disconnectAll()
{
    // for (std::map<AddressCouple, ConnectorCouples>::iterator i = m_nConnections.begin(); i != m_nConnections.end(); i++) {
        // for (auto t = i->begin(); t != i->end(); t++) {
    //         Connector* conr = t->first;
    //         Connection* conn = t->second;
    //         conn->shutdownWrite();
    //         conr->stop();
    //         delete conr;
    //         delete conn;
    //     }
    // }

    m_nConnections.clear();
}

void ClientPool::stop()
{
    disconnectAll();
    m_nStarted = false;
}

ClientPool::~ClientPool()
{
    stop();

    LOG(Debug) << "class ClientPool destructor\n";
}
