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

#ifndef SERVER_H
#define SERVER_H

#include <tiny_core/accepter.h>
#include <tiny_core/callback.h>
#include <tiny_core/channel.h>
#include <tiny_core/netaddress.h>

#include <boost/function.hpp>
#include <map>
#include <set>
#include <vector>

class EventLoop;
class Connection;
class Factory;

class Server {
private:
    bool m_nStarted;
    unsigned long long m_nConNum;
    NetAddress m_nListenAddress;
    EventLoop* m_pEventLoop;
    Factory* m_pFactory;
    Accepter m_nAccepter;
    std::set<Connection*> m_nConnections;

    ConnectionCallback m_nConnectCallback;
    MessageCallback m_nMessageCallback;
    WriteCompleteCallback m_nWriteCompleteCallback;
    CloseCallback m_nCloseCallback;

    void m_fHandleRead(int, const NetAddress&);
    void m_fHandleClose(Connection*);

public:
    Server(EventLoop*, const NetAddress&, int listenSocket, Factory*);
    void setConnectCallback(ConnectionCallback c)
    {
        m_nConnectCallback = c;
    }
    void setMessageCallback(MessageCallback c)
    {
        m_nMessageCallback = c;
    }
    void setWriteCompleteCallback(WriteCompleteCallback c)
    {
        m_nWriteCompleteCallback = c;
    }
    void setCloseCallback(CloseCallback c)
    {
        m_nCloseCallback = c;
    }
    void start();
    ~Server();
};

#endif // !SERVER_H
