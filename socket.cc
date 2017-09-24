#include "socket.h"
#include "log.h"
#include "api.h"

#include <unistd.h>


Socket::Socket(int fd)
{
    m_nFd = fd;
    LOG(Debug) << "class Socket constructor\n";
}

Socket::~Socket()
{
    close(m_nFd);
    LOG(Debug) << "class Socket destructor\n";
}