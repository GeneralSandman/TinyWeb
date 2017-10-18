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

//----------Protocol api----------//
// Protocol::Protocol()
// {
//     LOG(Debug) << "class Protocol constructor\n";
// }

// Protocol::~Protocol()
// {
//     LOG(Debug) << "class Protocol destructor\n";
// }

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

//------DiscardProtocol api-------------//
DiscardProtocol::DiscardProtocol()
{
    LOG(Debug) << "class DiscardProtocol constructor\n";
}

void DiscardProtocol::connectionMade()
{
    std::cout << "[Discard] new connection\n";
}

void DiscardProtocol::dataReceived(const std::string &)
{
    std::cout << "[Discard] data redeived\n";
}

void DiscardProtocol::connectionLost()
{
    std::cout << "[Discard] lost connection\n";
}

DiscardProtocol::~DiscardProtocol()
{
    LOG(Debug) << "class DiscardProtocol destructor\n";
}

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

//--------WebProtocol api-------------//
WebProtocol::WebProtocol()
{
    LOG(Debug) << "class WebProtocol constructor\n";
}

void WebProtocol::m_fResponse(Connection *con, struct HttpRequest &request)
{
    std::string h = "HTTP/1.0 200 OK\r\n";
    std::string c = "Content-Type: text/html\r\n\r\n";
    std::string html = "hello world";

    std::string res = h + c + html;
    con->send(res);
    con->shutdownWrite();
    //If we don't close this connection,
    //the html can't be showed in browser.
    //[Http protocol]

    printHttpRequest(request);
}

void WebProtocol::connectionMade(Connection *con)
{
    std::cout << "(WebProtocol) "
              << "get a new connection\n";
}

void WebProtocol::dataReceived(Connection *con, Buffer *input, Time time)
{
    std::cout << "(WebProtocol) "
              << "get a new message\n";
    //class Parser to parse the data received;

    std::string line;
    int i = 0;
    struct HttpRequest request;

    while (input->getALine(line))
    {
        // std::cout << i++ << "-" << line << "-\n";
        if (m_nParser.parseRequestLine(line, request))
        {
            m_fResponse(con, request);
        }

        line = "";
    }
}

void WebProtocol::connectionLost(Connection *con)
{
    std::cout << "(WebProtocol) "
              << "lost a connection\n";
}

WebProtocol::~WebProtocol()
{
    LOG(Debug) << "class WebProtocol destructor\n";
}