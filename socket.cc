#include "socket.h"
#include "netaddress.h"
#include "log.h"
#include "api.h"

#include <unistd.h>
#include <string.h>

Socket::Socket(int fd)
{
    m_nFd = fd;
    LOG(Debug) << "class Socket constructor\n";
}

void Socket::bindAddress(NetAddress &address)
{
    int len;
    struct sockaddr_in s = address.getAddr();
    Bind(m_nFd, &s);
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

Socket::~Socket()
{
    close(m_nFd);
    LOG(Debug) << "class Socket destructor\n";
}