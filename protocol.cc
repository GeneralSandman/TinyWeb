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

#include "protocol.h"
#include "connection.h"
#include "log.h"

#include <vector>

std::string
getName(const Protocol *p)
{
    std::stringstream s;
    s << typeid(*p).name();
    std::string tmp(s.str().c_str());

    std::string res;
    for (int i = 0; i < tmp.size(); i++)
        if (!(tmp[i] >= 48 && tmp[i] <= 57))
            res += tmp[i];
    return res;
}

//----------Reflect api----------//
std::map<std::string, createProtocol> Reflect::m_nProts;

Protocol *getInstanceByPointer(const Protocol *p)
{
    return Reflect::getReflectInstance().getProtocolByName(getName(p));
}
//----------Protocol api----------//
Protocol::Protocol()
{
    LOG(Debug) << "class Protocol constructor\n";
}

void Protocol::makeConnection()
{
    //It is used by Factory
    connectionMade();
}

void Protocol::getMessage(const std::string &data)
{
    //It is used by Factory
    dataReceived(data);
}

void Protocol::loseConnection()
{
    //It is used by Factory
    connectionLost();
}

void Protocol::sendMessage(const std::string &data)
{
    m_pConnection->send(data);
}

void Protocol::connectionMade()
{
    //It can be override
}

void Protocol::dataReceived(const std::string &data)
{
    //It can be override
}

void Protocol::connectionLost()
{
    //It can be override
}

Protocol::~Protocol()
{
    LOG(Debug) << "class Protocol destructor\n";
}

RegistProtocol(Protocol);

//------DiscardProtocol api-------------//
DiscardProtocol::DiscardProtocol()
{
    LOG(Debug) << "class DiscardProtocol constructor\n";
}

void DiscardProtocol::connectionMade()
{
    std::cout << "[Discard] new connection\n";
}

void DiscardProtocol::dataReceived(const std::string &data)
{
    std::cout << "[Discard] data redeived:" << data << "\n";
}

void DiscardProtocol::connectionLost()
{
    std::cout << "[Discard] lost connection\n";
}

DiscardProtocol::~DiscardProtocol()
{
    LOG(Debug) << "class DiscardProtocol destructor\n";
}

RegistProtocol(DiscardProtocol);

//------EchoProtocol api-------------//
EchoProtocol::EchoProtocol()
    : Protocol()
{
    LOG(Debug) << "class EchoProtocol constructor\n";
}

void EchoProtocol::connectionMade()
{
    std::cout << "[Echo] new Connection\n";
}

void EchoProtocol::dataReceived(const std::string &data)
{
    std::cout << "[Echo] get data:" << data << "\n";
    sendMessage(data);
}

void EchoProtocol::connectionLost()
{
    std::cout << "[Echo] lost Connection\n";
}

EchoProtocol::~EchoProtocol()
{
    LOG(Debug) << "class EchoProtocol destructor\n";
}

RegistProtocol(EchoProtocol);

//--------WebProtocol api-------------//
WebProtocol::WebProtocol()
    : m_nResponser(this)
{
    LOG(Debug) << "class WebProtocol constructor\n";
}

void WebProtocol::m_fResponse(struct HttpRequest &request)
{
    std::string h = "HTTP/1.0 200 OK\r\n";
    std::string c = "Content-Type: text/html\r\n\r\n";
    std::string html = "hello world";

    std::string res = h + c + html;
    sendMessage(res);
    m_pConnection->shutdownWrite();
    //If we don't close this connection,
    //the html can't be showed in browser.
    //[Http protocol]
}

void WebProtocol::connectionMade()
{
    std::cout << "[WebProtocol] "
              << "get a new connection\n";
}

void WebProtocol::dataReceived(const std::string &data)
{
    std::cout << "[WebProtocol] "
              << "get a new message\n";
    struct HttpRequest request;
    // std::cout << "-" << data << "-\n";
    if (m_nParser.parseRequest(data, request))
    {
        // printHttpRequest(request);
        m_nResponser.response(request);
    }
}

void WebProtocol::connectionLost()
{
    std::cout << "[WebProtocol] "
              << "lost a connection\n";
}

WebProtocol::~WebProtocol()
{
    LOG(Debug) << "class WebProtocol destructor\n";
}

RegistProtocol(WebProtocol);
