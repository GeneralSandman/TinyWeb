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
#include "factory.h"
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
    // LOG(Debug) << "class Protocol constructor\n";
}

void Protocol::makeConnection()
{
    //It is used by Factory
    //user can't change this function.
    connectionMade();
}

void Protocol::getMessage(const std::string &data)
{
    //It is used by Factory
    //user can't change this function.
    dataReceived(data);
}

void Protocol::writeComplete()
{
    //It is used by Factory
    //user can't change this function.
    writeCompletely();
}

void Protocol::loseConnection()
{
    //It is used by Factory
    //user can't change this function.
    connectionLost();
}

void Protocol::sendMessage(const std::string &data)
{
    m_pConnection->send(data);
}

void Protocol::closeProtocol()
{
    m_pFactory->closeProtocol(this);
}

void Protocol::closeProtocolAfter(int seconds)
{
    m_pFactory->closeProtocolAfter(this, seconds);
}

void Protocol::connectionMade()
{
    //It can be override user.
}

void Protocol::dataReceived(const std::string &data)
{
    //It can be override by user.
}

void Protocol::writeCompletely()
{
    //It can be override by user.
}

void Protocol::connectionLost()
{
    //It can be override by user.
}

Protocol::~Protocol()
{
    // LOG(Debug) << "class Protocol destructor\n";
}

RegistProtocol(Protocol);

//----------end Protocol api----------//

//------DiscardProtocol api-------------//
DiscardProtocol::DiscardProtocol()
{
    LOG(Debug) << "class DiscardProtocol constructor\n";
}

void DiscardProtocol::connectionMade()
{
    LOG(Info) << "new connection\n";
}

void DiscardProtocol::dataReceived(const std::string &data)
{
    LOG(Info) << "data redeived\n";
}

void DiscardProtocol::connectionLost()
{
    LOG(Info) << "lost connection\n";
}

DiscardProtocol::~DiscardProtocol()
{
    LOG(Debug) << "class DiscardProtocol destructor\n";
}

RegistProtocol(DiscardProtocol);

//------end-DiscardProtocol api-------------//

//------EchoProtocol api-------------//
EchoProtocol::EchoProtocol()
    : Protocol()
{
    LOG(Debug) << "class EchoProtocol constructor\n";
}

void EchoProtocol::connectionMade()
{
    LOG(Info) << "new Connection \n";
}

void EchoProtocol::dataReceived(const std::string &data)
{
    LOG(Info) << "get data \n";
    sendMessage(data);
}

void EchoProtocol::connectionLost()
{
    LOG(Info) << "lost Connection \n";
}

EchoProtocol::~EchoProtocol()
{
    LOG(Debug) << "class EchoProtocol destructor\n";
}

RegistProtocol(EchoProtocol);

//------end-EchoProtocol api-------------//

//--------WebProtocol api-------------//

WebProtocol::WebProtocol()
    : m_nKeepAlive(false),
      m_nResponser(this)
{
    LOG(Debug) << "class WebProtocol constructor\n";
}

void WebProtocol::connectionMade()
{
    LOG(Info) << "get a new connection\n";
}

void WebProtocol::dataReceived(const std::string &data)
{
    LOG(Info) << "get a message\n";
    struct HttpRequest request;
    struct HttpResponse response;

    if (m_nParser.parseRequest(data, request))
    {
        LOG(Info) << "request url<" << request.line.url << ">\n";
        m_nResponser.buildHttpResponse(request, response);
        m_nResponser.sendResponse(response);
        LOG(Info) << "response url<" << request.line.url << ">\n";
        if (request.header.connection == "keep-alive")
            closeProtocolAfter(10);
        else
            closeProtocol();
    }
    else
    {
        //The request message has format error
        closeProtocol();
    }
}

void WebProtocol::connectionLost()
{
    LOG(Info) << "lost a connection\n";
}

WebProtocol::~WebProtocol()
{
    LOG(Debug) << "class WebProtocol destructor\n";
}

RegistProtocol(WebProtocol);

//--------end-WebProtocol api-------------//
