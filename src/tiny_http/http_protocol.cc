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
#include <tiny_http/http_proxyer.h>

// WebProtocol using for web server.

WebProtocol::WebProtocol()
    : Protocol()
    , m_nPool()
    , m_nKeepAlive(false)
    , m_nBufferSize(4 * 1024)
    , m_nMaxChainSize(10 * 4 * 1024)
    , m_pFileChain(nullptr)
    , m_nUseWriteCompleteCallback(false)
    , m_nBeginSendFile(false)
    , m_nResponser(&m_nPool)
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

    int begin = 0;
    int valid;
    bool valid_requ;
    m_nBeginSendHeader = false;
    m_nBeginSendFile = false;
    m_nUseWriteCompleteCallback = false;
    m_pRequest = std::make_shared<HttpRequest>();
    m_pResponse = std::make_shared<HttpResponse>();

    m_nParser.setType(HTTP_TYPE_REQUEST);
    valid = m_nParser.execute(data.c_str(),
        begin,
        data.size(),
        m_pRequest.get());
    valid_requ = (valid == -1) ? false : true;

    // TODO:update
    // 1. new responser
    // 2. build http-resp-header
    // 3. build http-resp-body {write by memory or sendfile}
    // 4. write http-resp-header
    // 5. write http-resp-body
    // 6. if http-file is too big. setting write-complete callback.
    //    write the reset of file.

    sdstr str;
    sdsnewempty(&str);

    m_nResponser.buildResponse(m_pRequest.get(), valid_requ, m_pResponse.get());

    // Malloc space for write file.
    unsigned int file_size = m_pResponse->file.getFileSize();
    unsigned int buffer_num = 0;
    if (file_size && file_size <= m_nMaxChainSize) {
        buffer_num = file_size / m_nBufferSize;
        if (file_size % m_nBufferSize) {
            buffer_num++;
        }
    } else if (file_size && file_size > m_nMaxChainSize) {
        buffer_num = m_nMaxChainSize / m_nBufferSize;
    }
    m_pFileChain = m_nPool.getNewChain(buffer_num);
    m_nPool.mallocSpace(m_pFileChain, m_nBufferSize);

    // Encode http response.
    m_nResponser.lineToStr(&(m_pResponse->line), &str);
    m_nResponser.headersToStr(&(m_pResponse->headers), &str);
    LOG(Debug) << "all-buffer-size:" << countAllBufferSize(m_pFileChain)
               << ",all-nodeal-size:" << countAllNoDealSize(m_pFileChain) << std::endl;
    m_pWriteChain = m_nResponser.bodyToChain(&(m_pResponse->file), m_pFileChain, content_no_t, transport_chunked_t);
    LOG(Debug) << "all-buffer-size:" << countAllBufferSize(m_pWriteChain)
               << ",all-nodeal-size:" << countAllNoDealSize(m_pWriteChain) << std::endl;

    // Rest data of file need to send or not.
    if (!m_nResponser.noMoreBody(&(m_pResponse->file), m_pFileChain, content_no_t, transport_chunked_t)) {

        auto func
            = [this]() mutable -> int {
            LOG(Info) << "write rest of file\n";
            HttpFile* file = &(m_pResponse->file);

            // clearData(m_pFileChain);
            LOG(Debug) << "all-buffer-size:" << countAllBufferSize(m_pFileChain)
                       << ",all-nodeal-size:" << countAllNoDealSize(m_pFileChain) << std::endl;
            m_pWriteChain = m_nResponser.bodyToChain(file, m_pFileChain, content_no_t, transport_chunked_t);
            LOG(Debug) << "all-buffer-size:" << countAllBufferSize(m_pWriteChain)
                       << ",all-nodeal-size:" << countAllNoDealSize(m_pWriteChain) << std::endl;
            // if (file->noMoreData()) {
            //     std::cout << "no more data\n";
            //     m_nUseWriteCompleteCallback = false;
            // }
            if (m_nResponser.noMoreBody(file, m_pFileChain, content_no_t, transport_chunked_t)) {
                std::cout << "no more data\n";
                m_nUseWriteCompleteCallback = false;
            }
            sendChain(m_pWriteChain);
            clearData(m_pWriteChain);

            return 0;
        };

        std::cout << "Set write complete callback\n";
        m_fWriteRestFile = func;
        m_nUseWriteCompleteCallback = true;
    }

    std::string tmp;
    tmp.append((const char*)str.data, str.len);
    sendMessage(tmp);
    sendChain(m_pWriteChain);
    clearData(m_pWriteChain);

    sdsdestory(&str);
}

void WebProtocol::getDataFromProxy()
{
    // 
    int valid;
    char* str;
    unsigned int begin;
    unsigned int len;

    HttpRequest* request = new HttpRequest;
    m_nParser.execute(str, begin, len, request);

    int * test = new int;
    *test = 90090;
    auto connectCallback = [test]() {
        LOG(Debug) << "WebProtocol connect source-server by Proxyer\n";

        LOG(Debug) << "Debug int value:" << *test << std::endl;
        // Create HttpRequest.
        // Send request to source-server.
    };

    auto messageCallback = [test]() {
        LOG(Debug) << "WebProtocol get message from source-server by Proxyer\n";

        // Create HttpParser.
        // Parse respnse from source-server.
        LOG(Debug) << "Debug int value:" << *test << std::endl;
    };

    auto writeCompleteCallback = [test]() {
        LOG(Debug) << "WebProtocol write completely source-server by Proxyer\n";

        LOG(Debug) << "Debug int value:" << *test << std::endl;
    };

    auto closeCallback = [test]() {
        LOG(Debug) << "WebProtocol close connection with source-server by Proxyer\n";
        // do nothing
        LOG(Debug) << "Debug int value:" << *test << std::endl;
    };


    // m_nProxyer request something of source-sever
    // static-site-proxyer or dynamic-response-fcgi
    proxy_callback_t * proxy_callback = new proxy_callback_t;
    proxy_callback->connectCallback = connectCallback; // send proxy request.
    proxy_callback->messageCallback = messageCallback; // parse proxy response.
    proxy_callback->writeCompleteCallback = writeCompleteCallback;
    proxy_callback->closeCallback = closeCallback;


    delete test;
    delete request;
}

void WebProtocol::writeCompletely()
{
    LOG(Info) << "write complete:"
              << m_nBeginSendHeader << " "
              << m_nBeginSendFile << std::endl;

    // if (!m_nBeginSendHeader) {
    // m_nBeginSendHeader = true;
    // return;
    // }

    if (!m_nBeginSendFile) {
        // update this when sendMessage completely.
        m_nBeginSendFile = true;
        return;
    }

    // write the reset of file.
    if (!m_nUseWriteCompleteCallback) {
        // m_fWriteRestFile = nullptr;
        return;
    }

    m_fWriteRestFile();
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
    , m_nPool()
    , m_nKeepAlive(false)
    , fcgiModel(1314) // FIXME:
{
    LOG(Debug) << "class FcgiClientProtocol constructor\n";
}

void FcgiClientProtocol::connectionMade()
{
    // TODO: how to pass param.
    LOG(Info) << "get a new connection\n";

    // Write fcgi request.
    // http_header header;
    // std::string cgiargs = "name=zhenhuli&age=99";
    std::string content;
    std::string requestData;

    // memset((void*)&header, 0, sizeof(http_header));
    // strcpy(header.uri, "");
    // strcpy(header.method, "GET");
    // strcpy(header.version, "HTTP/1.1");
    // strcpy(header.filename, "/var/www/html/test/dynamic_get.php");
    // strcpy(header.name, "");
    // strcpy(header.cgiargs, cgiargs.c_str());
    // strcpy(header.contype, "");
    // strcpy(header.conlength, "0");

    // fcgiModel.buildFcgiRequest(&header, content, requestData);
    fcgiModel.buildFcgiRequest(&m_nRequestHeader, content, requestData);

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
        HttpBuilder responser(&m_nPool);
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

// NewFcgiClientProtocol using for new fcgi client.

NewFcgiClientProtocol::NewFcgiClientProtocol()
    : Protocol()
    , m_nPool()
    , m_nKeepAlive(false)
    , fcgiModel(1314) // FIXME:
{
    LOG(Debug) << "class NewFcgiClientProtocol constructor\n";
}

void NewFcgiClientProtocol::connectionMade()
{
    // TODO: how to pass param.
    LOG(Info) << "get a new connection\n";

    // Write fcgi request.
    http_header header;
    std::string cgiargs = "name=zhenhuli&age=99";
    std::string content;
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

    fcgiModel.buildFcgiRequest(&header, content, requestData);
    // fcgiModel.buildFcgiRequest(&m_nRequestHeader, content, requestData);

    sendMessage(requestData);
}

void NewFcgiClientProtocol::dataReceived(const std::string& data)
{
    LOG(Info) << "NewFcgiClientProtocol get data\n";

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
        HttpBuilder responser(&m_nPool);
        std::string data;

        responser.response(result, data);

        // Funciton of basic class.
        // send message to client.
        // sendMessage(data);
    }

    delete result;
}

void NewFcgiClientProtocol::connectionLost()
{
    LOG(Info) << "lost a connection\n";
}

NewFcgiClientProtocol::~NewFcgiClientProtocol()
{
    LOG(Debug) << "class NewFcgiClientProtocol destructor\n";
}

RegistProtocol(NewFcgiClientProtocol);

// FcgiClientProtocol
