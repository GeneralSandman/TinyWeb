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

#include <tiny_core/socket.h>
#include <tiny_core/netaddress.h>
#include <tiny_base/log.h>
#include <tiny_base/api.h>

#include <unistd.h>
#include <string.h>

Socket::Socket(int fd)
{
    m_nFd = fd;
    m_nClosed = false;
    setSocketReuseAddress(fd); //SO_REUSEADDR
    LOG(Debug) << "class Socket constructor\n";
}

void Socket::bindAddress(const NetAddress &address)
{
    struct sockaddr_in s = address.getAddr();
    Bind(m_nFd, &s);
    //setSocketReuseAddress(m_nFd);
}
void Socket::listen()
{
    Listen(m_nFd, 100);
}

int Socket::accept(NetAddress &per)
{
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    int connectfd = Accept(m_nFd, &addr);
    per.setAddr(addr);
    return connectfd;
}

int Socket::connect(const NetAddress &per)
{
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr = per.getAddr();
    return Connect(m_nFd, &addr);
}

void Socket::shutdownWrite()
{
    ShutdownWrite(m_nFd);
}

void Socket::close()
{
    if (m_nClosed == false)
    {
        Close(m_nFd);
        m_nClosed = true;
    }
}

void Socket::setNoDelay()
{
    //disable tcp Nagle algorithm.
    disableTcpDelay(m_nFd);
}

void Socket::setDelay()
{
    //I think this function has bug,
    //FIXME:
    enableTcpDelay(m_nFd);
}

void Socket::setKeepAlive()
{
    enableTcpKeepAlive(m_nFd);
}

void Socket::setNoKeepAlive()
{
    //I think this function has bug,
    //FIXME:
    disableTcpKeepAlive(m_nFd);
}

Socket::~Socket()
{
    if (m_nClosed == false)
    {
        Close(m_nFd);
        m_nClosed = true;
    }
    LOG(Debug) << "class Socket destructor\n";
}