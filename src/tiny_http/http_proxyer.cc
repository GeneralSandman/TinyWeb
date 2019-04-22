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

#include <tiny_base/log.h>
#include <tiny_core/clientpool.h>
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

    std::string request_str1 = "GET http://172.17.0.3:80/tinyweb/tinyweb_home.html HTTP/1.0\r\n"
                               "User-Agent: Mozilla/5.0 "
                               "(compatible; MSIE 9.0; "
                               "Windows NT 6.1; WOW64; Trident/5.0)\r\n"
                               "\r\n";

    std::string request_str2 = "GET http://172.17.0.3:80/404/404.html HTTP/1.1\r\n"
                               "Host: 172.17.0.3\r\n"
                               "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_1) "
                               "AppleWebKit/605.1.15 (KHTML, like Gecko) "
                               "Version/12.0.1 Safari/605.1.15\r\n"
                               "\r\n";
    sendMessage(request_str2);

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
    // return data to HttpParser or HttpBuilder.
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
    const NetAddress& clientAddress)
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
    NetAddress serverAddress("172.17.0.3:80"); // Init by configer.
    m_pClientPool->addClient(m_nClientAddress, serverAddress, false, false);
}

void HttpProxyer::stop()
{
    m_pClientPool->stop();
}

void HttpProxyer::request(const NetAddress& serverAddress, proxy_callback_t* callbacks)
{
    HttpProxyProtocol* prot = new HttpProxyProtocol();
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
