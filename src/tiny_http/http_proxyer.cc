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

#include <tiny_base/clientpool.h>
#include <tiny_base/log.h>
#include <tiny_core/eventloop.h>
#include <tiny_core/netaddress.h>
#include <tiny_core/protocol.h>
#include <tiny_http/http_proxyer.h>

//----------HttpProxyProtocol api----------//

HttpProxyProtocol::HttpProxyProtocol()
{
    LOG(Debug) << "class HttpProxyProtocol constructor\n";
}

void HttpProxyProtocol::connectionMade()
{
    LOG(Info) << "HttpProxyProtocol connection made\n";

    if (nullptr != m_pCallbacks) {
        LOG(Info) << "invoke connectCallback of m_pCallbacks\n";
        m_pCallbacks->connectCallback();
    }
}

void HttpProxyProtocol::dataReceived(const std::string& data)
{
    LOG(Info) << "HttpProxyProtocol data received:"
              << data << std::endl;

    if (nullptr != m_pCallbacks) {
        LOG(Info) << "invoke messageCallback of m_pCallbacks\n";
        m_pCallbacks->messageCallback();
    }

    // Receive data from source web server.
    // Parser data
    // justice cache body or not
    // return data to HttpParser or HttpResponser.
}

void HttpProxyProtocol::writeCompletely()
{
    LOG(Info) << "HttpProxyProtocol write completely\n";

    if (nullptr != m_pCallbacks) {
        LOG(Info) << "invoke writeCompleteCallback of m_pCallbacks\n";
        m_pCallbacks->writeCompleteCallback();
    }
}

void HttpProxyProtocol::connectionLost()
{
    LOG(Info) << "HttpProxyProtocol connection lost\n";

    if (nullptr != m_pCallbacks) {
        LOG(Info) << "invoke closeCallback of m_pCallbacks\n";
        m_pCallbacks->closeCallback();
    }
}

HttpProxyProtocol::~HttpProxyProtocol()
{
    LOG(Debug) << "class HttpProxyProtocol destructor\n";
}

RegistProtocol(HttpProxyProtocol);

//----------end HttpProxyProtocol api----------//

HttpProxyer::HttpProxyer(EventLoop* loop,
    const NetAddress& clientAddress,
    Protocol* protocol)
    : m_pEventLoop(loop)
    , m_nClientAddress(clientAddress)
    , m_pClientPool(new ClientPool(loop, clientAddress))
{
    LOG(Debug) << "class HttpProxyer constructor\n";
}

void HttpProxyer::start()
{
    // Read configure from file
    // and invoke addClient()
    
    m_pClientPool->start();
    m_pClientPool->addClient();
}

void HttpProxyer::stop()
{
    m_pClientPool->stop();
}

void HttpProxyer::request(const NetAddress& serverAddress, const proxy_callback_t* callbacks)
{
    Protocol* prot = new HttpProxyProtocol();
    prot->setProxyCallback(callbacks);
    m_nProtocols.push_back(prot);

    m_pClientPool->doTask(m_nClientAddress, serverAddress, prot);
}

HttpProxyer::~HttpProxyer()
{
    delete m_pClientPool;

    for (auto t : m_nProtocols) {
        delete t;
    }
    LOG(Debug) << "class HttpProxyer destructor\n";
}
