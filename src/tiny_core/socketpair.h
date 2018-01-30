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
    void setParentSocket(EventLoop *loop);
    void setChildSocket(EventLoop *loop);
    void writeToChild(const std::string &data);
    void writeToParent(const std::string &data);

    void setConnectCallback(const ConnectionCallback &c);
    void setMessageCallback(const MessageCallback &c);
    void setWriteCompleteCallback(const WriteCompleteCallback &c);
    void setCloseCallback(const CloseCallback &c);

    void clearSocket();
    ~SocketPair();
};

#endif //!SOCKET_PAIR_H