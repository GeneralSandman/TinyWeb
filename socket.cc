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
    Bind(m_nFd, (const struct sockaddr *)(&s));
}
void Socket::listen()
{
    Listen(m_nFd, 100);
}

int Socket::accept(NetAddress &res)
{
    struct sockaddr addr;
    bzero(&addr, sizeof(addr));
    int len;
    int connectfd = accept(m_nFd, &addr, &len);
    res.setAddr((struct sockaddr_in)addr);
    
}

Socket::~Socket()
{
    close(m_nFd);
    LOG(Debug) << "class Socket destructor\n";
}