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

#ifndef HTTP_PROXYER_H
#define HTTP_PROXYER_H

#include <tiny_core/clientpool.h>
#include <tiny_core/connection.h>
#include <tiny_core/netaddress.h>
#include <tiny_core/protocol.h>

#include <map>

class EventLoop;

typedef struct proxy_callback_t {
    ConnectionCallback connectCallback;
    MessageCallback messageCallback;
    WriteCompleteCallback writeCompleteCallback;
    CloseCallback closeCallback;
} proxy_callback_t;

class HttpProxyProtocol : public ClientPoolProtocol {
private:
    proxy_callback_t* m_pCallbacks;

public:
    HttpProxyProtocol();

    inline void setProxyCallback(const proxy_callback_t* callback)
    {
        m_pCallbacks = callback;
    }

    void connectionMade();
    void dataReceived(const std::string& data);
    void writeCompletely();
    void connectionLost();

    virtual ~HttpProxyProtocol();
}

class HttpProxyer {
private:
    // std::map<NetAddress, Connection *> m_nServers;
    // std::map<NetAddress, Connection *> m_nClients;

    // Server m_nServer;
    // Client m_nClient;
    // bool m_nStarted;

    EventLoop* m_pEventLoop;
    ClientPool* m_pClientPool;
    Protocol* m_pProtocol;
    std::vector<Protocol*> m_nProtocols;

    ConnectionCallback m_nConnectCallback;
    MessageCallback m_nMessageCallback;
    WriteCompleteCallback m_nWriteCompleteCallback;
    CloseCallback m_nCloseCallback;

public:
    HttpProxyer(EventLoop* loop,
        const NetAddress& clientAddress,
        Protocol* protocol);
    void start();
    void stop();

    void request(const std::string& requ);

    ~HttpProxyer();
};

#endif // !HTTP_PROXYER_H
