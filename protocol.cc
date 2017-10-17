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
#include "log.h"

Protocol::Protocol()
{
    LOG(Debug) << "class Protocol constructor\n";
}

Protocol::~Protocol()
{
    LOG(Debug) << "class Protocol destructor\n";
}

//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////

WebProtocol::WebProtocol()
{
    LOG(Debug) << "class WebProtocol constructor\n";
}

void WebProtocol::m_fResponse(HttpRequestHeader *header, HttpRequestContent *content)
{
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
    while (input->getALine(line))
    {
        std::cout << i++ << "-" << line << "-\n";
        struct HttpRequestHeader header;
        struct HttpRequestContent content;
        if (m_nParser.parseRequestLine(line, &header, &content))
        {
            // m_fResponse(&header, &content);
            // writeHtml(con->get,"/home/li/TinyWeb/www/index.html");

            std::string h = "HTTP/1.0 200 OK\r\n";
            std::string c = "%sContent-Type: text/html\r\n\r\n";
            std::string html = "hello world";

            std::string res = h + c + html;
            con->send(res);
            con->shutdownWrite();
            //If we don't close this connection,
            //the html can't be showed in browser.
            //[Http protocol]
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