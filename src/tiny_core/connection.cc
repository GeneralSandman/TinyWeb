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
#include <tiny_base/buffer.h>
#include <tiny_core/channel.h>
#include <tiny_core/connection.h>
#include <tiny_core/eventloop.h>
#include <tiny_core/netaddress.h>
#include <tiny_core/socket.h>
#include <tiny_core/time.h>

#include <boost/bind.hpp>
#include <iostream>

void Connection::m_fHandleRead(Time arrive)
{
    ssize_t n = m_nInputBuffer.put(m_pChannel->getFd());

    if (n > 0) {
        if (m_nMessageCallback)
            m_nMessageCallback(this,
                &m_nInputBuffer,
                arrive);
        // LOG(Debug) << arrive.toString() << std::endl;
    } else if (n == 0) {
        m_fHandleClose();
    } else {
        m_fHandleError();
    }
}

void Connection::m_fHandleWrite()
{
    if (m_pChannel->isWriting()) {
        std::string send = m_nOutputBuffer.getAll();
        size_t n = writeString(m_pChannel->getFd(),
            send);
        if (n > 0) {
            if (m_nOutputBuffer.readableBytes() == 0) {
                m_pChannel->disableWrite(); //it's very important,you can try to delete the if
                if (m_nState == DisConnecting)
                    m_fShutdownWrite();
                if (m_nWriteCompleteCallback)
                    m_nWriteCompleteCallback(this);
            } else {
                //have more data to write
            }
        }

        else {
            //FIXME:need handle error.
        }
    } else {
        //connection close down ,don't write
        LOG(Warn) << "connection close down ,don't write\n";
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
    LOG(Warn) << "socket error\n";
}

void Connection::m_fShutdownWrite()
{
    if (m_nState == Connected) //FIXME:
        m_nState = DisConnecting;
    if (!m_pChannel->isWriting())
        m_pConnectSocket->shutdownWrite();
}

Connection::Connection(EventLoop* loop,
    int connectfd,
    const NetAddress& local,
    const NetAddress& peer)
    : m_pEventLoop(loop)
    , m_nState(Connecting)
    , m_pConnectSocket(new Socket(connectfd))
    , m_pChannel(new Channel(loop, connectfd))
    , m_nLocalAddress(local)
    , m_nPeerAddress(peer)
{
    //m_pConnectSocket->bindAddress(m_nLocalAddress);
    m_pChannel->setReadCallback(boost::bind(&Connection::m_fHandleRead, this, _1));
    m_pChannel->setWriteCallback(boost::bind(&Connection::m_fHandleWrite, this));
    m_pChannel->setCloseCallback(boost::bind(&Connection::m_fHandleClose, this));
    m_pChannel->setErrorCallback(boost::bind(&Connection::m_fHandleError, this));

    LOG(Info) << "get connection " << m_nPeerAddress.getIpPort() << std::endl;

    LOG(Debug) << "class Connection constructor\n";
}

void Connection::send(const std::string& message)
{
    ssize_t nwrote = 0;

    if (!m_pChannel->isWriting() && m_nOutputBuffer.readableBytes() == 0) {

        nwrote = ::write(m_pChannel->getFd(), message.data(), message.size());
        if (nwrote >= 0) {
            if ((unsigned int)nwrote == message.size()) {
                //write complete
                m_pChannel->disableWrite();
                if (m_nWriteCompleteCallback)
                    m_nWriteCompleteCallback(this);
            } else {
                //have more data to write,
                //output buffer will write rest data.
            }
        } else {
            nwrote = 0;
            //FIXME:handle error.
        }
    }

    if ((unsigned int)nwrote < message.size()) {
        m_nOutputBuffer.append(message.data() + nwrote, message.size() - nwrote);
        if (!m_pChannel->isWriting())
            m_pChannel->enableWrite();
    }
}

void Connection::send(buffer_t* buffer)
{
    if (nullptr == buffer) {
        return;
    }

    ssize_t nwrote = 0;
    unsigned int data_size = 0;
    data_size = buffer->used - buffer->deal;
    if (!m_pChannel->isWriting() && m_nOutputBuffer.readableBytes() == 0) {

        // FIXME: count data-size from buffer->deal.
        nwrote = ::write(m_pChannel->getFd(), buffer->deal, data_size);
        if (nwrote >= 0) {

            if ((unsigned int)nwrote == data_size) {
                //write complete
                m_pChannel->disableWrite();
                if (m_nWriteCompleteCallback)
                    m_nWriteCompleteCallback(this);
            } else {
                // TODO
                // have more data to write,
                // output buffer will write rest data.
                // place rest data to out-buffer.
            }
        } else {
            nwrote = 0;
            // FIXME:handle error.
        }
    }

    buffer->deal += (unsigned int)nwrote;

    if ((unsigned int)nwrote < data_size) {
        m_nOutputBuffer.append((const char*)buffer->deal, buffer->used - buffer->deal);
        if (!m_pChannel->isWriting())
            m_pChannel->enableWrite();
    }
}

void Connection::send(const chain_t* chain)
{
    if (nullptr == chain) {
        return;
    }

    const chain_t* tmp_chain = chain;
    buffer_t* buffer = nullptr;

    ssize_t nwrote = 0;
    unsigned int data_size = 0;

    while (tmp_chain) {
        buffer = tmp_chain->buffer;
        if (nullptr == buffer) {
            tmp_chain = tmp_chain->next;
            continue;
        }
        data_size = buffer->used - buffer->deal;

        if (!m_pChannel->isWriting() && m_nOutputBuffer.readableBytes() == 0) {

            // FIXME: count data-size from buffer->deal.
            nwrote = ::write(m_pChannel->getFd(), buffer->deal, data_size);
            if (nwrote >= 0) {

                if ((unsigned int)nwrote == data_size
                    && (buffer->islast || nullptr == tmp_chain->next)) {
                    //write complete
                    m_pChannel->disableWrite();
                    if (m_nWriteCompleteCallback)
                        m_nWriteCompleteCallback(this);
                } else {
                    // TODO
                    // have more data to write,
                    // output buffer will write rest data.
                    // place rest data to out-buffer.
                }
            } else {
                nwrote = 0;
                // FIXME:handle error.
            }
        }

        buffer->deal += (unsigned int)nwrote;

        if ((unsigned int)nwrote < data_size) {
            m_nOutputBuffer.append((const char*)buffer->deal, buffer->used - buffer->deal);
            if (!m_pChannel->isWriting())
                m_pChannel->enableWrite();
        }

        tmp_chain = tmp_chain->next;
    }
}

void Connection::sendfile(int inputFd, off_t* offset, size_t count)
{
    // TODO: how to identify boundary with outputBuffer.
}

void Connection::establishConnection()
{
    m_nState = Connected;
    m_pChannel->enableRead();
    if (m_nConnectCallback)
        m_nConnectCallback(this);
}

void Connection::destoryConnection()
{
    //this function always be used after shutdown or lost connection
    //to destory some data of connection
    //after invoking this function, we delete this connection.

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
    if (m_pConnectSocket != nullptr) {
        delete m_pConnectSocket;
        m_pConnectSocket = nullptr;
    }
    if (m_pChannel != nullptr) {
        delete m_pChannel;
        m_pChannel = nullptr;
    }

    LOG(Debug) << "class Connection destructor\n";
}
