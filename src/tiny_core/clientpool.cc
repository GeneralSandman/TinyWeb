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
    if (p != m_nProtocols.end()) {
        Protocol* prot = p->second;
        // prot->makeConnection();
    }
}

void ClientPool::m_nMessageCallback(Connection* con,
    Buffer* input, Time time)
{
    LOG(Info) << "invoke message-callback of Protocol\n";

    auto p = m_nProtocols.find(con);
    if (p != m_nProtocols.end()) {
        Protocol* prot = p->second;

        std::string all = input->getAll();
        prot->getMessage(all);
    } else {
        LOG(Debug) << "zhenhuli\n";
    }
}

void ClientPool::m_nWriteCompleteCallback(Connection* con)
{
    LOG(Info) << "invoke writecomplete-callback of Protocol\n";

    auto p = m_nProtocols.find(con);
    if (p != m_nProtocols.end()) {
        Protocol* prot = p->second;

        prot->writeComplete();
    }
}

void ClientPool::m_nCloseCallback(Connection* con)
{
    LOG(Info) << "invoke close-callback of Protocol\n";

    auto p = m_nProtocols.find(con);
    if (p != m_nProtocols.end()) {
        Protocol* prot = p->second;

        prot->loseConnection();

        LOG(Debug) << "erase <Connection(" << con->getLocalAddress().getIpPort() 
            << "--" << con->getPeerAddress().getIpPort() << "),Protocol>\n";
        m_nProtocols.erase(p);
    }
}

void ClientPool::m_fNewConnectionCallback(int sockfd,
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

    // version 3
    // Maybe some bugs.
    assert(m_nConnections2.find(add) != m_nConnections2.end());
    client_t* client = nullptr;
    auto i = m_nConnections2[add].begin();
    auto end = m_nConnections2[add].end();
    for (; i != end; i++) {
        client = *i;
        if (client->connector->getSocketFd() == sockfd) {
            m_nConnections2[add].erase(i);
            break;
        }
        client = nullptr;
    }

    if (nullptr == client) {
        LOG(Warn) << "find connection error\n";
        return;
    }
    assert(client->connection == nullptr);
    client->connection = newCon;
    client->valid = true;
    client->isusing = false;
    m_nConnections2[add].push_front(client);
}

void ClientPool::m_fHandleClose(Connection* con)
{
    m_nCloseCallback(con);

    AddressCouple add(con->getLocalAddress(), con->getPeerAddress());
    auto p = m_nConnections2.find(add);
    assert(p != m_nConnections2.end());

    client_t* client = nullptr;
    auto i = m_nConnections2[add].begin();
    auto end = m_nConnections2[add].end();
    for (; i != end; i++) {
        client = *i;
        if (client->connection == con) {
            m_nConnections2[add].erase(i);
            break;
        }
        client = nullptr;
    }

    if (nullptr == client) {
        LOG(Warn) << "find connection error\n";
        return;
    }

    Connector* conr = client->connector;
    Connection* conn = client->connection;

    client->connection = nullptr;
    client->valid = false;
    client->isusing = false;

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

void ClientPool::m_fGiveUpControl(Connection* con)
{
    LOG(Info) << "give up control\n";

    AddressCouple address(con->getLocalAddress(), con->getPeerAddress());

    if (m_nConnections2.find(address) == m_nConnections2.end()) {
        return;
    }

    client_t* client = nullptr;
    auto i = m_nConnections2[address].begin();
    for (; i != m_nConnections2[address].end(); i++) {
        client = *i;

        if (client->connection == con) {
            m_nConnections2[address].erase(i);
            break;
        }
        client = nullptr;
    }
    if (nullptr == client) {
        return;
    }

    client->isusing = false;
    m_nConnections2[address].push_front(client);
}

void ClientPool::m_fWakeUp(Connection* con)
{
    LOG(Info) << "weak up protocol in wait-list\n";

    auto i = m_nWaitList.begin();
    auto end = m_nWaitList.end();
    for (; i != end; i++) {

        LOG(Debug) << "old-connection(host:" << con->getLocalAddress().getIpPort()
                   << ",peer:" << con->getPeerAddress().getIpPort() << ")\n";
        LOG(Debug) << "wait-list-connection(host:" << i->first.first.getIpPort()
                   << ",peer:" << i->first.second.getIpPort() << ")\n";

        if (i->first.first == con->getLocalAddress()
            && i->first.second == con->getPeerAddress()) {

            LOG(Debug) << "find match protocol from wait-list\n";

            AddressCouple address(con->getLocalAddress(), con->getPeerAddress());
            Protocol* protocol = i->second;
            bool res = m_fDoTaskNoDelay(m_nConnections2[address], protocol);
            if (!res) {
                LOG(Warn) << "big error\n";
            }

            m_nWaitList.erase(i);
            break;
        }
    }
    if (i == end) {
        LOG(Debug) << "don't find matched waitting protocol from wait-list\n";
        return;
    }
}

bool ClientPool::m_fDoTaskNoDelay(ConnectorCouples2& couples, Protocol* protocol)
{
    LOG(Info) << "do task with no-delay\n";

    auto i = couples.begin();
    client_t* client = nullptr;

    for (; i != couples.end(); i++) {
        client = *i;
        std::cout << client << "-" << client->valid << "-" << client->isusing << std::endl;
        if (client != nullptr && client->valid && !client->isusing) {
            couples.erase(i);
            break;
        }
        client = nullptr;
    }

    if (nullptr == client) {
        LOG(Warn) << "There are no idle connection\n";
        return false;
    }

    client->isusing = true;
    couples.push_back(client);

    Connection* con = client->connection;

    protocol->m_pFactory = nullptr;
    protocol->m_nNumber = 1;
    protocol->m_pConnection = con;
    m_nProtocols[con] = protocol;

    // disableConnection in close protocol.
    con->establishConnection();
    protocol->makeConnection();
    buildProtocol(protocol);

    return true;
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
    LOG(Debug) << "close Protocol\n";
    // Protocol give up control to connection.

    auto p = m_nProtocols.find(protocol->m_pConnection);
    if (p != m_nProtocols.end()) {
        Connection* con = p->first;
        Protocol* prot = p->second;


        // con->shutdownWrite();
        prot->loseConnection();
        LOG(Debug) << "erase <Connection(" << con->getLocalAddress().getIpPort()
            << "--" << con->getPeerAddress().getIpPort() << "),Protocol>\n";
        m_nProtocols.erase(p);

        m_fGiveUpControl(con);
        m_fWakeUp(con);

        // delete prot;
        LOG(Debug) << "erase protocol\n";
    } else {
        LOG(Debug) << "erase protocol error\n";
    }
}

void ClientPool::closeProtocolAfter(Protocol* protocol, int seconds)
{
    m_pEventLoop->runAfter(seconds, boost::bind(&ClientPool::closeProtocol, this, protocol));
}

void ClientPool::addClient(const NetAddress& hostAddress,
    const NetAddress& peerAddress,
    bool retry,
    bool keepConnect,
    int num)
{
    for (int i = 0; i < num; i++) {
        connect(hostAddress, peerAddress, retry, keepConnect);
    }
}

void ClientPool::doTask(const NetAddress& hostAddress,
    const NetAddress& peerAddress,
    Protocol* protocol)
{
    AddressCouple address(hostAddress, peerAddress);
    if (m_nConnections2.find(address) == m_nConnections2.end()) {
        LOG(Info) << "no have connection, need to build new one\n";
        return;
    }

    bool res = m_fDoTaskNoDelay(m_nConnections2[address], protocol);

    if (!res) {
        // There no idle connection.
        // Push to wait list.
        LOG(Info) << "push to wait-list(host:" << hostAddress.getIpPort()
                  << ",peer:" << peerAddress.getIpPort() << ")\n";
        MultiProtocol tmp(address, protocol);
        m_nWaitList.push_back(tmp);
    }
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
    client_t* newClient = new client_t;
    client_init(newClient);
    newClient->connector = newConnector;
    m_nConnections2[address].push_back(newClient);

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

    m_nConnections2.clear();
}

void ClientPool::stop()
{
    disconnectAll();
    m_nStarted = false;
}

ClientPool::~ClientPool()
{
    disconnectAll();

    LOG(Debug) << "class ClientPool destructor\n";
}
