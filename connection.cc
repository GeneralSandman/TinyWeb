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

#include "connection.h"
#include "netaddress.h"
#include "eventloop.h"
#include "socket.h"
#include "channel.h"
#include "time.h"
#include "api.h"
#include "log.h"

#include <boost/bind.hpp>
#include <iostream>

void Connection::m_fHandleRead(Time arrive)
{
    ssize_t n = m_nInputBuffer.put(m_pChannel->getFd());

    if (n > 0)
    {
        if (m_nMessageCallback)
            m_nMessageCallback(this,
                               &m_nInputBuffer,
                               arrive);
    }
    else if (n == 0)
    {
        m_fHandleClose();
    }
    else
    {
        m_fHandleError();
    }
}

void Connection::m_fHandleWrite()
{
    if (m_pChannel->isWriting())
    {
        std::string send = m_nOutputBuffer.getAll();
        size_t n = writeString(m_pChannel->getFd(),
                               send);
        if (n > 0)
        {
            std::cout << "write " << n << " Bytes\n";
            if (m_nOutputBuffer.readableBytes() == 0)
            {
                m_pChannel->disableWrite(); //it's very important,you can try to delete the if
                if (m_nState == DisConnecting)
                    m_fShutdownWrite();
            }
            else
            {
                //have more data to write
            }
        }

        else
        {
            //error
            std::cout << "write error\n";
        }
    }
    else
    {
        //connection close down ,don't write
        std::cout << "connection close down ,don't write\n";
    }
}

void Connection::m_fHandleClose()
{
    m_pChannel->disableAll();
    if (m_nCloseCallback)
        m_nCloseCallback(this);
}

void Connection::m_fHandleError()
{
    std::cout << "socket error\n";
}

void Connection::m_fShutdownWrite()
{
    if (m_nState == Connected) //FIXME:
        m_nState = DisConnecting;
    if (!m_pChannel->isWriting())
        m_pConnectSocket->shutdownWrite();
}

Connection::Connection(EventLoop *loop,
                       int connectfd,
                       const NetAddress &local,
                       const NetAddress &peer)
    : m_pEventLoop(loop),
      m_nState(Connecting),
      m_pConnectSocket(new Socket(connectfd)),
      m_pChannel(new Channel(loop, connectfd)),
      m_nLocalAddress(local),
      m_nPeerAddress(peer)
{
    m_pChannel->setReadCallback(boost::bind(&Connection::m_fHandleRead, this, _1));
    m_pChannel->setWriteCallback(boost::bind(&Connection::m_fHandleWrite, this));
    m_pChannel->setCloseCallback(boost::bind(&Connection::m_fHandleClose, this));
    m_pChannel->setErrorCallback(boost::bind(&Connection::m_fHandleError, this));

    LOG(Debug) << "class Connection constructor\n";
}

void Connection::send(const std::string &message)
{
    ssize_t nwrote = 0;
    if (!m_pChannel->isWriting() && m_nOutputBuffer.readableBytes() == 0)
    {
        nwrote = ::write(m_pChannel->getFd(), message.data(), message.size());
        if (nwrote >= 0)
        {
        }
        else
        {
            nwrote = 0;
            //error
        }
    }

    if (nwrote < message.size())
    {
        m_nOutputBuffer.append(message.data() + nwrote, message.size() - nwrote);
        if (!m_pChannel->isWriting())
            m_pChannel->enableWrite();
    }
}

void Connection::establishConnection()
{
    m_nState = Connected;
    m_pChannel->enableRead();
    m_nConnectCallback(this);
}

void Connection::destoryConnection()
{
    m_nState = DisConnected;
    m_pChannel->disableAll();
    m_pEventLoop->removeChannel(m_pChannel);

    delete m_pConnectSocket;
    delete m_pChannel;

    m_pConnectSocket = nullptr;
    m_pChannel = nullptr;
}

Connection::~Connection()
{
    if (m_pConnectSocket != nullptr)
    {
        delete m_pConnectSocket;
        m_pConnectSocket = nullptr;
    }
    if (m_pChannel != nullptr)
    {
        delete m_pChannel;
        m_pChannel = nullptr;
    }

    LOG(Debug) << "class Connection destructor\n";
}
