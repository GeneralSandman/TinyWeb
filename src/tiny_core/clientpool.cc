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
#include <tiny_core/protocol.h>

#include <boost/bind.hpp>

void ClientPool::m_nConnectCallback(Connection* con)
{
    LOG(Info) << "invoke connect-callback of Protocol\n";

    auto p = m_nProtocols.find(con);
    if (p != m_nProtocols.end()) {
        Protocol* prot = p->second;

        // prot->makeConnection();
    } else {
        LOG(Info) << "m_nProtocol has not corresponding connection->protocol pair\n";
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
        LOG(Warn) << "m_nProtocol has not corresponding connection->protocol pair\n";
    }
}

void ClientPool::m_nWriteCompleteCallback(Connection* con)
{
    LOG(Info) << "invoke writecomplete-callback of Protocol\n";

    auto p = m_nProtocols.find(con);
    if (p != m_nProtocols.end()) {
        Protocol* prot = p->second;

        prot->writeComplete();
    } else {
        LOG(Warn) << "m_nProtocol has not corresponding connection->protocol pair\n";
    }
}

void ClientPool::m_nCloseCallback(Connection* con)
{
    LOG(Info) << "invoke close-callback of Protocol\n";

    auto p = m_nProtocols.find(con);
    if (p != m_nProtocols.end()) {
        Protocol* prot = p->second;

        prot->loseConnection();

        LOG(Info) << "erase <Connection(" << con->getLocalAddress().getIpPort()
                  << "--" << con->getPeerAddress().getIpPort() << "),Protocol>\n";

        // delete prot;
        prot = nullptr;
        m_nProtocols.erase(p);
    } else {
        LOG(Info) << "m_nProtocol has not corresponding connection->protocol pair\n";
    }
}

void ClientPool::m_fNewConnectionCallback(int sockfd,
    const NetAddress& hostAddress,
    const NetAddress& peerAddress) //this arg may be error.
{
    // This function must identifer localaddress to
    // establish connection.
    Connection* newCon = new Connection(m_pEventLoop,
        sockfd,
        hostAddress,
        peerAddress);
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
            &ClientPool::m_nWriteCompleteCallback,
            this, _1));
    CloseCallback tmp_nCloseCallback = CloseCallback(
        boost::bind(
            &ClientPool::m_fHandleClose,
            this, _1));

    newCon->setConnectCallback(tmp_nConnectCallback);
    newCon->setMessageCallback(tmp_nMessageCallback);
    newCon->setWriteCompleteCallback(tmp_nWriteCompleteCallback);
    newCon->setCloseCallback(tmp_nCloseCallback);

    // Add this new Connection to m_nConnections.
    std::string peerAddress_str = peerAddress.getIpPort();
    if (m_nConnections.find(peerAddress_str) == m_nConnections.end()) {
        LOG(Warn) << "m_nConnection haven't pre-connection:" << peerAddress_str << std::endl;
        return;
    }
    client_t* client = nullptr;
    auto i = m_nConnections[peerAddress_str].begin();
    auto end = m_nConnections[peerAddress_str].end();
    for (; i != end; i++) {
        client = *i;
        if (nullptr != client && nullptr != client->connector
            && client->connector->getSocketFd() == sockfd) {
            m_nConnections[peerAddress_str].erase(i);
            break;
        }
        client = nullptr;
    }

    if (nullptr == client) {
        LOG(Warn) << "m_nConnection has not corresponding client to store new connection\n";
        return;
    }
    assert(client->connection == nullptr);
    client->connection = newCon;
    client->valid = true;
    client->isusing = false;
    m_nConnections[peerAddress_str].push_front(client);

    newCon->establishConnection();

    m_fWakeUp(newCon);
}

void ClientPool::m_fHandleClose(Connection* con)
{
    LOG(Debug) << "handle close of connection======\n";
    m_nCloseCallback(con);

    NetAddress peerAddress = con->getPeerAddress();
    std::string peerAddress_str = peerAddress.getIpPort();
    auto p = m_nConnections.find(peerAddress_str);
    assert(p != m_nConnections.end());

    client_t* client = nullptr;
    auto i = m_nConnections[peerAddress_str].begin();
    auto end = m_nConnections[peerAddress_str].end();
    for (; i != end; i++) {
        client = *i;
        if (nullptr != client && nullptr != client->connection
            && client->connection == con) {
            m_nConnections[peerAddress_str].erase(i);
            LOG(Debug) << "erase invalid connection(" << peerAddress_str << ")\n";
            break;
        }
        client = nullptr;
    }

    if (nullptr == client) {
        LOG(Warn) << "m_nConnection has not corresponding client to clear closed connection\n";
        return;
    }

    auto j = m_nProtocols.find(con);
    if (j != m_nProtocols.end()) {
        Protocol* prot = m_nProtocols[con];

        prot->loseConnection();
        LOG(Debug) << "zhenhuli\n";
        // delete prot;
        j->second = nullptr;
        m_nProtocols.erase(j);
    }

    Connector* conr = client->connector;
    Connection* conn = client->connection;

    client->connection = nullptr;
    client->valid = false;
    client->isusing = false;

    conr->stop();
    conn->destoryConnection();

    // TODO: ???
    delete conn;
    if (!conr->isKeepConnect()) { //can't be reused.
        delete conr;
    } else { //This pair may be reused next time.
    }

    if (m_nStarted && conr->isKeepConnect())
        conr->restart();

    
    // Check there are tasks which not be handled in
    // WaitList.
    if (m_nWaitList.size()) {
        auto i = m_nWaitList.begin();
        NetAddress peerAddress = i->first;
        Protocol* prot = i->second;

        // FIXME: init retry and keep-connect.
        m_fConnect(m_nHostAddress, peerAddress, false, false);
    }
}

void ClientPool::m_fGiveUpControl(Connection* con)
{
    LOG(Info) << "protocol give up control to connection\n";

    NetAddress peerAddress = con->getPeerAddress();
    std::string peerAddress_str = peerAddress.getIpPort();

    if (m_nConnections.find(peerAddress_str) == m_nConnections.end()) {
        return;
    }

    client_t* client = nullptr;
    auto i = m_nConnections[peerAddress_str].begin();
    for (; i != m_nConnections[peerAddress_str].end(); i++) {
        client = *i;
        if (nullptr != client && nullptr != client->connection
            && client->connection == con) {
            m_nConnections[peerAddress_str].erase(i);
            break;
        }
        client = nullptr;
    }

    if (nullptr == client) {
        LOG(Warn) << "m_nConnection has not corresponding client->connection to give up  control\n";
        return;
    }

    client->isusing = false;
    m_nConnections[peerAddress_str].push_front(client);
}

void ClientPool::m_fWakeUp(Connection* con)
{
    LOG(Info) << "weak up protocol in wait-list\n";

    NetAddress peerAddress = con->getPeerAddress();
    std::string peerAddress_str = peerAddress.getIpPort();

    auto i = m_nWaitList.begin();
    auto end = m_nWaitList.end();
    for (; i != end; i++) {

        LOG(Debug) << "old-connection(host:" << con->getLocalAddress().getIpPort()
                   << ",peer:" << con->getPeerAddress().getIpPort() << ")\n";

        if (i->first == peerAddress) {

            LOG(Debug) << "find corresponding protocol from wait-list\n";

            Protocol* protocol = i->second;
            bool res = m_fDoTaskNoDelay(m_nConnections[peerAddress_str], protocol);
            if (!res) {
                LOG(Warn) << "big error\n";
            }

            m_nWaitList.erase(i);
            break;
        }
    }
    if (i == end) {
        LOG(Debug) << "m_nWaitList hasn't corresponding protocol\n";
        return;
    }
}

bool ClientPool::m_fDoTaskNoDelay(ConnectorCouples& couples, Protocol* protocol)
{
    LOG(Info) << "do task with no-delay\n";

    auto i = couples.begin();
    client_t* client = nullptr;

    for (; i != couples.end(); i++) {
        client = *i;
        LOG(Debug) << "Connection:" << client->connection << "-" << client->valid << "-" << client->isusing << std::endl;
        if (nullptr != client
            && client->valid && !client->isusing) {
            couples.erase(i);
            break;
        }
        client = nullptr;
    }

    if (nullptr == client) {
        LOG(Warn) << "m_nConnection has not idle connection to handle protocol\n";
        return false;
    }

    client->isusing = true;
    couples.push_back(client);

    Connection* con = client->connection;
    // Protocol* prot = getInstanceByPointer(protocol);
    Protocol* prot = protocol;
    prot->m_pFactory = nullptr;
    prot->m_nNumber = 1;
    prot->m_pConnection = con;

    ClientPoolProtocol* newProt = dynamic_cast<ClientPoolProtocol*>(prot);
    if (nullptr != newProt) {
        LOG(Debug) << "It is ClientPoolProtocol\n";
        newProt->setClientPool(this);
    }

    m_nProtocols[con] = prot;
    LOG(Debug) << "m_nProtocols size:" << m_nProtocols.size() << std::endl;

    // disableConnection in close protocol.
    // con->establishConnection();
    prot->makeConnection();
    buildProtocol(prot);

    return true;
}

void ClientPool::m_fConnect(const NetAddress& hostAddress,
    const NetAddress& peeraddress,
    bool retry,
    bool keepconnect)
{

    Connector* newConnector = new Connector(m_pEventLoop,
        hostAddress,
        peeraddress,
        retry,
        keepconnect);
    newConnector->setConnectionCallback(
        boost::bind(
            &ClientPool::m_fNewConnectionCallback,
            this, _1, _2, _3));

    client_t* newClient = new client_t;
    client_init(newClient);
    newClient->connector = newConnector;
    m_nConnections[peeraddress.getIpPort()].push_back(newClient);

    newConnector->start();

    LOG(Info) << "pre-connection:" << peeraddress.getIpPort() << std::endl;
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
    LOG(Debug) << "close Protocol to give up control of Connection\n";

    auto p = m_nProtocols.find(protocol->m_pConnection);
    if (p != m_nProtocols.end()) {

        Connection* con = p->first;
        Protocol* prot = p->second;

        // con->shutdownWrite();
        prot->loseConnection();
        LOG(Debug) << "erase <Connection(" << con->getLocalAddress().getIpPort()
                   << "--" << con->getPeerAddress().getIpPort() << "),Protocol>\n";
        LOG(Debug) << "m_nProtocols size:" << m_nProtocols.size() << std::endl;
        LOG(Debug) << "Connection:" << con << ",Protocol:" << prot << std::endl;

        // delete prot;
        p->second = nullptr;
        m_nProtocols.erase(p);

        m_fGiveUpControl(con);
        m_fWakeUp(con);

        // delete prot;
    } else {
        LOG(Warn) << "m_nProtocol has not corresponding connection->protocol pair\n";
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
        m_fConnect(hostAddress, peerAddress, retry, keepConnect);
    }
}

void ClientPool::doTask(const NetAddress& hostAddress,
    const NetAddress& peerAddress,
    Protocol* protocol)
{
    std::string peerAddress_str = peerAddress.getIpPort();

    LOG(Debug) << "doTask serverAddress(" << peerAddress_str << ")\n";

    // ----
    // if (m_nConnections.find(peerAddress_str) == m_nConnections.end()
    //     || m_nConnections[peerAddress_str].empty()) {
    //     LOG(Info) << "m_nConnections hasn't corresponding connection, need to build new one\n";

    //     m_fConnect(hostAddress, peerAddress, false, false);
    //     return;
    // }

    // bool res = m_fDoTaskNoDelay(m_nConnections[peerAddress_str], protocol);

    // if (!res) {
    //     LOG(Info) << "push to wait-list(host:" << hostAddress.getIpPort()
    //               << ",peer:" << peerAddress.getIpPort() << ")\n";
    //     MultiProtocol tmp(peerAddress, protocol);
    //     m_nWaitList.push_back(tmp);
    // }

    // ------
    bool res = false;
    if (m_nConnections.find(peerAddress_str) != m_nConnections.end()
        && m_nConnections[peerAddress_str].size()) {
        
        res = m_fDoTaskNoDelay(m_nConnections[peerAddress_str], protocol);
    }

    if (!res) {
        LOG(Info) << "push to wait-list(host:" << hostAddress.getIpPort()
                  << ",peer:" << peerAddress.getIpPort() << ")\n";
        MultiProtocol tmp(peerAddress, protocol);
        m_nWaitList.push_back(tmp);
    }

    if (m_nConnections.find(peerAddress_str) == m_nConnections.end()
        || m_nConnections[peerAddress_str].empty()) {
        LOG(Info) << "m_nConnections hasn't corresponding connection, need to build new one\n";

        // FIXME: init retry and keep-connect.
        m_fConnect(hostAddress, peerAddress, false, false);
        return;
    }
}

void ClientPool::start()
{
    m_nStarted = true;
}

void ClientPool::disconnect(const NetAddress& peeraddress,
    const NetAddress& hostaddress)
{
}

void ClientPool::disconnectAll()
{
    for (auto i = m_nProtocols.begin(); i != m_nProtocols.end(); i++) {
        Protocol* prot = i->second;
        prot->loseConnection();

        // delete prot;
        i->second = nullptr;
    }
    m_nProtocols.clear();

    client_t* client = nullptr;
    for (auto i = m_nConnections.begin(); i != m_nConnections.end(); i++) {
        for (auto p = i->second.begin(); p != i->second.end(); p++) {
            client = *p;
            if (nullptr == client) {
                continue;
            }
            Connector* conr = client->connector;
            Connection* conn = client->connection;

            if (nullptr != conr)
                conr->stop();

            if (nullptr != conn)
                conn->destoryConnection();

            delete conr;
            delete conn;
            delete client;
        }
    }
    m_nConnections.clear();

    for (auto i = m_nWaitList.begin(); i != m_nWaitList.end(); i++) {
        Protocol* prot = i->second;

        // delete prot;
        i->second = nullptr;
    }
    m_nWaitList.clear();
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
