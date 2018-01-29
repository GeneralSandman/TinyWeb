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

#ifndef SOCKET_PAIR_H
#define SOCKET_PAIR_H

#include <tiny_base/api.h>
#include <tiny_core/connection.h>

#include <unistd.h>
#include <assert.h>
#include <cstring>

class SocketPair
{
  private:
    int m_nFds[2];
    //parent using 0,child using 1
    EventLoop *m_pEventLoop;
    Connection *m_pConnection;

    bool m_nIsFork;
    bool m_nIsParent;

  public:
    SocketPair();
    void createSocket();
    void setParentSocket(EventLoop *loop)
    {
        m_pEventLoop = loop;
        Close(m_nFds[1]);
        m_nIsFork = true;
        m_nIsParent = true;
        std::cout << "switch parent:" << getpid() << std::endl;
        NetAddress tmp;
        m_pConnection = new Connection(m_pEventLoop,m_nFds[0],
                                       tmp,tmp);
    }
    void setChildSocket(EventLoop *loop)
    {
        m_pEventLoop = loop;
        Close(m_nFds[0]);
        m_nIsFork = true;
        m_nIsParent = false;
        std::cout << "switch child:" << getpid() << std::endl;
        NetAddress tmp;
        m_pConnection = new Connection(m_pEventLoop,m_nFds[1],
                                       tmp,tmp);
    }
    void writeToChild(const std::string &data)
    {
        assert(m_nIsFork);
        assert(m_nIsParent);
        m_pConnection->send(data);
    }
    void writeToParent(const std::string &data)
    {
        assert(m_nIsFork);
        assert(!m_nIsParent);
        m_pConnection->send(data);
    }
    void setReadCallback(const MessageCallback &c)
    {
        m_pConnection->setMessageCallback(c);
    }
    ~SocketPair();
};

#endif //!SOCKET_PAIR_H