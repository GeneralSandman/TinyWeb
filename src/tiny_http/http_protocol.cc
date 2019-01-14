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
#include <tiny_core/protocol.h>
#include <tiny_http/http_model_fcgi.h>
#include <tiny_http/http_parser.h>
#include <tiny_http/http_protocol.h>
#include <tiny_http/http_responser.h>

// WebProtocol using for web server.

WebProtocol::WebProtocol()
    : Protocol()
    , m_nKeepAlive(false)
{
    LOG(Debug) << "class WebProtocol constructor\n";
}

void WebProtocol::connectionMade()
{
    LOG(Info) << "get a new connection\n";
}

void WebProtocol::dataReceived(const std::string& data)
{
    LOG(Info) << "WebProtocol get data\n";

    HttpParserSettings settings;
    HttpRequest* result = new HttpRequest;
    int begin = 0;

    HttpParser parser(&settings);
    parser.setType(HTTP_TYPE_REQUEST);

    int tmp = parser.execute(data.c_str(),
        begin,
        data.size(),
        result);

    bool res = (tmp == -1) ? false : true;
    if (res) {
        HttpResponser responser;
        std::string data;

        responser.response(result, data);

        // Funciton of basic class.
        sendMessage(data);
    }

    delete result;
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

// WebProtocol

// FcgiClientProtocol using for fcgi client.

FcgiClientProtocol::FcgiClientProtocol()
    : Protocol()
    , m_nKeepAlive(false)
    , fcgiModel(1314)
{
    LOG(Debug) << "class FcgiClientProtocol constructor\n";
}

void FcgiClientProtocol::connectionMade()
{
    LOG(Info) << "get a new connection\n";

    // Write fcgi request.
    http_header header;
    std::string cgiargs = "name=zhenhuli&age=99";
    std::string requestData;

    memset((void*)&header, 0, sizeof(http_header));
    strcpy(header.uri, "");
    strcpy(header.method, "GET");
    strcpy(header.version, "HTTP/1.1");
    strcpy(header.filename, "/var/www/html/test/dynamic_get.php");
    strcpy(header.name, "");
    strcpy(header.cgiargs, cgiargs.c_str());
    strcpy(header.contype, "");
    strcpy(header.conlength, "0");

    fcgiModel.buildFcgiRequest(&header, requestData);
    sendMessage(requestData);

}

void FcgiClientProtocol::dataReceived(const std::string& data)
{
    LOG(Info) << "FcgiClientProtocol get data\n";

    // Read fcgi response.
    fcgiModel.parseFcgiResponse(data);

    HttpParserSettings settings;
    HttpRequest* result = new HttpRequest;
    int begin = 0;

    HttpParser parser(&settings);
    parser.setType(HTTP_TYPE_FCGI_RESPONSE);

    int tmp = parser.execute(data.c_str(),
        begin,
        data.size(),
        result);

    bool res = (tmp == -1) ? false : true;
    if (res) {
        HttpResponser responser;
        std::string data;

        responser.response(result, data);

        // Funciton of basic class.
        // send message to client.
        // sendMessage(data);
    }

    delete result;
}

void FcgiClientProtocol::connectionLost()
{
    LOG(Info) << "lost a connection\n";
}

FcgiClientProtocol::~FcgiClientProtocol()
{
    LOG(Debug) << "class FcgiClientProtocol destructor\n";
}

RegistProtocol(FcgiClientProtocol);

// FcgiClientProtocol
