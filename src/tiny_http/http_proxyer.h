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
#include <tiny_http/http_model_fcgi.h>

#include <map>

class EventLoop;
class HttpModelFcgi;

// TODO: update ProxyCallback
typedef boost::function<void()> ProxyCallback;

typedef struct proxy_callback_t {
    ProxyCallback connectCallback;
    ProxyCallback messageCallback;
    ProxyCallback writeCompleteCallback;
    ProxyCallback closeCallback;
} proxy_callback_t;

class HttpProxyProtocol : public ClientPoolProtocol {
private:

    HttpModelFcgi* m_pFcgiModel;

    proxy_callback_t* m_pCallbacks;

public:
    HttpProxyProtocol();

    inline void setProxyCallback(proxy_callback_t* callback)
    {
        m_pCallbacks = callback;
    }

    void connectionMade();
    void dataReceived(const std::string& data);
    void writeCompletely();
    void connectionLost();

    virtual ~HttpProxyProtocol();
};

class HttpProxyer {
private:
    // bool m_nStarted;

    EventLoop* m_pEventLoop;
    NetAddress m_nClientAddress;
    ClientPool* m_pClientPool;
    std::vector<Protocol*> m_nProtocols;

    ConnectionCallback m_nConnectCallback;
    MessageCallback m_nMessageCallback;
    WriteCompleteCallback m_nWriteCompleteCallback;
    CloseCallback m_nCloseCallback;

public:
    HttpProxyer(EventLoop* loop,const NetAddress& clientAddress);
    
    void start();
    void stop();

    void request(const NetAddress& serverAddress, proxy_callback_t* callbacks);

    ~HttpProxyer();
};

#endif // !HTTP_PROXYER_H
