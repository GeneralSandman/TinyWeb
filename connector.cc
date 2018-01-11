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

#include "eventloop.h"
#include "netaddress.h"
#include "connector.h"
#include "socket.h"
#include "timerid.h"
#include "api.h"
#include "log.h"

#include <boost/function.hpp>
#include <boost/bind.hpp>

void Connector::m_fConnect()
{
    assert(m_nState = Disconnected);

    if (m_pConnectSocket != nullptr)
    {
        delete m_pConnectSocket;
        m_pConnectSocket = nullptr;
        //This function is not the first time invoked.
        //we have to delete invail connect socket.
    }

    if (m_pConnectChannel != nullptr)
    {
        delete m_pConnectChannel;
        m_pConnectChannel = nullptr;
        //This function is not the first time invoked.
        //we have to delete invail connect socket.
    }

    m_pConnectSocket = new Socket(createNoBlockSocket());
    m_pConnectSocket->bindAddress(m_nHostAddress);
    int res = m_pConnectSocket->connect(m_nServerAddress);
    int savedErrno = (res == 0) ? 0 : errno;
    switch (savedErrno)
    {
    case 0:
        m_fEstablishConnection();
        //to set channel
        break;
    case ECONNREFUSED:
        m_fRetryConnect();
        break;
    case EFAULT:
        m_pConnectSocket->close();
        break;
    default:
        m_pConnectSocket->close();
        break;
    }
}

void Connector::m_fHandleWrite()
{
    //get peer tcp message
    std::cout << "Connector handle write\n";

    if (m_nState == Connecting)
    {
        //removeInvaildChannel
        int sockfd = m_pConnectChannel->getFd();
        int error = getSocketError(sockfd);
        m_fRemoveInvaildConnectChannel();

        if (error)
        {
            std::cout << "socket error:" << error << std::endl;
            m_fRetryConnect();
        }
        else
        {
            m_nState = Connected;
            if (m_nConnect)
            {
                m_nNewConnectionCallback(sockfd, m_nHostAddress, m_nServerAddress);
                //fix
            }
            else
            {
                //close this socket;
                m_pConnectSocket->close();
            }
        }
    }
}

void Connector::m_fHandleError()
{
    //handle error

    //log
    assert(m_nState == Connecting);
    int last_sockfd = m_pConnectChannel->getFd();
    m_fRemoveInvaildConnectChannel();
    //will delete m_pConnectChannel

    //and get error from last_sockfd
    int error = getSocketError(last_sockfd);
    std::cout << "socket error:" << error << std::endl;

    if (m_nRetry)
        m_fRetryConnect();
}

void Connector::m_fRemoveInvaildConnectChannel()
{
    //this scoket is Invail,
    //so we can't use this connect channel  again
    //remove and reset it.

    m_pConnectChannel->disableAll();
    m_pEventLoop->removeChannel(m_pConnectChannel);
    delete m_pConnectChannel;
    m_pConnectChannel = nullptr;
}

void Connector::m_fEstablishConnection()
{
    //set callback of Channel
    m_nState = Connecting;
    assert(m_pConnectChannel == nullptr);
    m_pConnectChannel = new Channel(m_pEventLoop,
                                    m_pConnectSocket->getFd());
    m_pConnectChannel->setWriteCallback(
        boost::bind(&Connector::m_fHandleWrite, this));
    m_pConnectChannel->setErrorCallback(
        boost::bind(&Connector::m_fHandleError, this));
    m_pConnectChannel->enableWrite();
}

void Connector::m_fRetryConnect()
{
    if (m_pConnectSocket)
        m_pConnectSocket->close();
    m_nState = Disconnected;

    if (m_nConnect)
    {
        //retry
        //like connect() function
        *m_pRetryTimer = m_pEventLoop->runAfter(m_nRetryTime / 1000.0,
                                                boost::bind(&Connector::m_fConnect,
                                                            this));
        m_nRetryTime = std::min(m_nRetryTime * 2, MaxRetryDelayMs);
    }
    else
    {
    }
    //we have to handle expirti time.
}

Connector::Connector(EventLoop *loop,
                     const NetAddress &hostaddress,
                     const NetAddress &peeraddress,
                     bool retry,
                     bool keepconnect)
    : m_pEventLoop(loop),
      m_pConnectSocket(nullptr),
      m_pConnectChannel(nullptr),
      m_nHostAddress(hostaddress),
      m_nServerAddress(peeraddress),
      m_nRetry(retry),
      m_nKeepConnect(keepconnect),
      m_nRetryTime(InitRetryDelayMs)
{
    // m_nConnectSocket.bindAddress(NetAddress(hostport));
    //we have to close this socket fd.
    LOG(Debug) << "class Connector constructor\n";
}

void Connector::start()
{
    //demiss this function
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
    if (m_pRetryTimer != nullptr)
        m_pEventLoop->cancelTimerId(*m_pRetryTimer);
}

Connector::~Connector()
{

    if (m_pConnectSocket != nullptr)
    {
        delete m_pConnectSocket;
        m_pConnectSocket = nullptr;
    }
    if (m_pConnectChannel != nullptr)
    {
        delete m_pConnectChannel;
        m_pConnectChannel = nullptr;
    }
    if (m_pRetryTimer != nullptr)
        m_pEventLoop->cancelTimerId(*m_pRetryTimer);

    LOG(Debug) << "class Connector destructor\n";
}