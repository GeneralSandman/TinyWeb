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

#include <tiny_core/socketpair.h>
#include <tiny_base/log.h>
#include <tiny_base/api.h>

#include <sys/socket.h>
#include <sys/types.h>

SocketPair::SocketPair()
    : m_nIsFork(false),
      m_nIsParent(true)
{
    LOG(Debug) << "class SocketPair constructor\n";
}

void SocketPair::createSocket()
{
    int res = socketpair(AF_UNIX, SOCK_STREAM, 0, m_nFds);
    if (res == -1)
        handle_error("socketpair error:");
}

void SocketPair::setParentSocket(EventLoop *loop)
{
    m_pEventLoop = loop;
    Close(m_nFds[1]);
    m_nIsFork = true;
    m_nIsParent = true;
    std::cout << "switch parent:" << getpid() << std::endl;
    NetAddress tmp;
    m_pConnection = new Connection(m_pEventLoop, m_nFds[0],
                                   tmp, tmp);
    m_pConnection->establishConnection();
}

void SocketPair::setChildSocket(EventLoop *loop)
{
    m_pEventLoop = loop;
    Close(m_nFds[0]);
    m_nIsFork = true;
    m_nIsParent = false;
    std::cout << "switch child:" << getpid() << std::endl;
    NetAddress tmp;
    m_pConnection = new Connection(m_pEventLoop, m_nFds[1],
                                   tmp, tmp);
    m_pConnection->establishConnection();
}

void SocketPair::writeToChild(const std::string &data)
{
    assert(m_nIsFork);
    assert(m_nIsParent);
    std::cout << "[parent] send data:" << data << std::endl;
    m_pConnection->send(data);
}

void SocketPair::writeToParent(const std::string &data)
{
    assert(m_nIsFork);
    assert(!m_nIsParent);
    std::cout << "[child] send data:" << data << std::endl;
    m_pConnection->send(data);
}

void SocketPair::setConnectCallback(const ConnectionCallback &c)
{
    m_pConnection->setConnectCallback(c);
}

void SocketPair::setMessageCallback(const MessageCallback &c)
{
    m_pConnection->setMessageCallback(c);
}

void SocketPair::setWriteCompleteCallback(const WriteCompleteCallback &c)
{
    m_pConnection->setWriteCompleteCallback(c);
}

void SocketPair::setCloseCallback(const CloseCallback &c)
{
    m_pConnection->setCloseCallback(c);
}

void SocketPair::clearSocket()
{
    //must be invoked before delete eventloop.
    m_pConnection->shutdownWrite();
    m_pConnection->destoryConnection();
    delete m_pConnection;
}

SocketPair::~SocketPair()
{
    LOG(Debug) << "class SocketPair destructor\n";
}
