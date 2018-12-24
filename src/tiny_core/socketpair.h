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

#ifndef SOCKET_PAIR_H
#define SOCKET_PAIR_H

#include <tiny_base/api.h>
#include <tiny_core/connection.h>

#include <assert.h>
#include <cstring>
#include <unistd.h>

class EventLoop;

class SocketPair {
private:
    //parent using 0,child using 1
    EventLoop* m_pEventLoop;
    int m_nFds[2];
    Connection* m_pConnection;

    bool m_nIsParent;

public:
    SocketPair(EventLoop* loop, int fds[2]);
    void setParentSocket(int port);
    void setChildSocket(int port);
    void writeToChild(const std::string& data);
    void writeToParent(const std::string& data);

    void setConnectCallback(const ConnectionCallback& c);
    void setMessageCallback(const MessageCallback& c);
    void setWriteCompleteCallback(const WriteCompleteCallback& c);
    void setCloseCallback(const CloseCallback& c);

    void clearSocket();
    ~SocketPair();
};

#endif //!SOCKET_PAIR_H
