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

SocketPair::~SocketPair()
{
    if (!m_nIsFork)
    {
        Close(m_nFds[0]);
        Close(m_nFds[1]);
    }
    else
    {
        if (m_nIsParent)
            Close(m_nFds[0]);
        else
            Close(m_nFds[1]);
    }
    LOG(Debug) << "class SocketPair destructor\n";
}
