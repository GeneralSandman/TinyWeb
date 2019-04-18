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
#include <tiny_core/connector.h>
#include <tiny_core/eventloop.h>
#include <tiny_core/netaddress.h>
#include <tiny_core/socket.h>
#include <tiny_core/timerid.h>

#include <boost/bind.hpp>
#include <boost/function.hpp>

void Connector::m_fConnect()
{
    assert(m_nState == Disconnected);

    // if (m_pConnectSocket != nullptr)
    // {
    // delete m_pConnectSocket;
    // m_pConnectSocket = nullptr;
    //This function is not the first time invoked.
    //we have to delete invail connect socket.
    // }

    if (m_pConnectChannel != nullptr) {
        delete m_pConnectChannel;
        m_pConnectChannel = nullptr;
    }

    // m_pConnectSocket = new Socket(createNoBlockSocket());
    // m_pConnectSocket->bindAddress(m_nHostAddress);
    // int res = m_pConnectSocket->connect(m_nServerAddress);

    m_nSockfd = createNoBlockSocket();
    setSocketReuseAddress(m_nSockfd); //TIME_WAIT
    struct sockaddr_in host = m_nHostAddress.getAddr();
    struct sockaddr_in peer = m_nServerAddress.getAddr();

    Bind(m_nSockfd, &host);
    int res = Connect(m_nSockfd, &peer);
    int savedErrno = (res == 0) ? 0 : errno;

    switch (savedErrno) {
    case 0:
    case EINPROGRESS:
    case EINTR:
    case EISCONN:
        m_fEstablishConnection();
        //to set channel
        break;

    case EAGAIN:
    case EADDRINUSE:
    case EADDRNOTAVAIL:
    case ECONNREFUSED:
    case ENETUNREACH:
        if (m_nRetry) {
            m_fRemoveInvaildConnectSocket();
            m_fRetryConnect();
        }
        break;

    case EACCES:
    case EPERM:
    case EAFNOSUPPORT:
    case EALREADY:
    case EBADF:
    case EFAULT:
    case ENOTSOCK:
        m_fRemoveInvaildConnectSocket();
        break;

    default:
        m_fRemoveInvaildConnectSocket();
        break;
    }
}

void Connector::m_fHandleWrite()
{
    //Handle write event after invoking connect() return zero.
    if (m_nState == Connecting) {
        //removeInvaildChannel
        int sockfd = m_pConnectChannel->getFd();
        int error = getSocketError(sockfd);
        //FIXME:why??????????????
        //If no error:we will not use connect-channel,because
        //      the connection have established.We don't those
        //      event.
        //Else with error:the connect-channel is invaild.
        // LOG(Debug) << "zhenhuli socket error:" << error << std::endl;
        if (error != 0) {
            m_fRemoveInvaildConnectChannel();
        }
        if (error) {
            if (m_nRetry)
                m_fRetryConnect();
        } else {
            m_nState = Connected;
            if (m_nConnect) {
                m_nNewConnectionCallback(sockfd, m_nHostAddress, m_nServerAddress);
            } else {
                //ignore this connection
                m_fRemoveInvaildConnectSocket();
            }
        }
    }
}

void Connector::m_fHandleError()
{
    //Handle error event after invoking connect() return zero.
    assert(m_nState == Connecting);

    int last_sockfd = m_pConnectChannel->getFd();
    m_fRemoveInvaildConnectChannel();
    m_fRemoveInvaildConnectSocket();
    //will delete m_pConnectChannel and m_pConnectSocket
    //and get error from last_sockfd
    int error = getSocketError(last_sockfd);
    LOG(Debug) << "socket error:" << error << std::endl;
    if (m_nRetry)
        m_fRetryConnect();
}

void Connector::m_fRemoveInvaildConnectChannel()
{
    m_pConnectChannel->disableAll();
    m_pEventLoop->removeChannel(m_pConnectChannel);
    delete m_pConnectChannel;
    m_pConnectChannel = nullptr;
}

void Connector::m_fRemoveInvaildConnectSocket()
{
    // assert(m_pConnectSocket != nullptr);
    // m_pConnectSocket->close();
    // delete m_pConnectSocket;
    // m_pConnectSocket = nullptr;

    Close(m_nSockfd);
    std::cout << "failed sockfd\n";
    m_nSockfd = -1;
}

void Connector::m_fEstablishConnection()
{
    //this function will finish some tasks after
    //invoking connect() return 0.
    //set callback of Channel
    m_nState = Connecting;
    assert(m_pConnectChannel == nullptr);
    m_pConnectChannel = new Channel(m_pEventLoop,
        m_nSockfd);
    //m_pConnectChannel is different from channel of Connection.
    //The events they inspect are different.
    m_pConnectChannel->setWriteCallback(
        boost::bind(&Connector::m_fHandleWrite, this));
    m_pConnectChannel->setErrorCallback(
        boost::bind(&Connector::m_fHandleError, this));
    m_pConnectChannel->enableWrite();
}

void Connector::m_fRetryConnect()
{
    m_nState = Disconnected;
    if (m_nConnect) {
        //like connect() function
        LOG(Debug) << "retry after " << m_nRetryTime << " seconds\n";
        m_nRetryTimer = m_pEventLoop->runAfter(m_nRetryTime / 1000.0,
            boost::bind(&Connector::m_fConnect,
                this));
        m_nRetryTime = std::min(m_nRetryTime * 2, MaxRetryDelayMs);
    } else {
    }
    //we have to handle expirti time.
}

Connector::Connector(EventLoop* loop,
    const NetAddress& hostaddress,
    const NetAddress& peeraddress,
    bool retry,
    bool keepconnect)
    : m_pEventLoop(loop)
    // , m_pConnectSocket(nullptr)
    , m_nSockfd(-1)
    , m_pConnectChannel(nullptr)
    , m_nHostAddress(hostaddress)
    , m_nServerAddress(peeraddress)
    , m_nState(Disconnected)
    , m_nRetry(retry)
    , m_nKeepConnect(keepconnect)
    , m_nRetryTime(InitRetryDelayMs)
    , m_nConnect(false)
{
    LOG(Debug) << "class Connector constructor\n";
}

void Connector::start()
{
    m_nConnect = true;
    m_fConnect();
}

void Connector::restart()
{
    m_nState = Disconnected;
    m_nRetryTime = InitRetryDelayMs;
    m_nConnect = true;
    m_fConnect();
}

void Connector::stop()
{
    m_nConnect = false;
    if (m_nRetryTimer.isValid()) //timer has been set correctly.
        m_pEventLoop->cancelTimerId(m_nRetryTimer);
}

Connector::~Connector()
{
    // if (m_pConnectSocket != nullptr)
    // {
    // delete m_pConnectSocket;
    // m_pConnectSocket = nullptr;
    // }
    // if (-1 != m_nSockfd)
    // Close(m_nSockfd);
    if (m_pConnectChannel != nullptr) {
        delete m_pConnectChannel;
        m_pConnectChannel = nullptr;
    }
    if (m_nRetryTimer.isValid())
        m_pEventLoop->cancelTimerId(m_nRetryTimer);

    LOG(Debug) << "class Connector destructor\n";
}
