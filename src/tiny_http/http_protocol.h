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

#ifndef HTTP_PROTOCOL_H
#define HTTP_PROTOCOL_H

#include <tiny_base/log.h>
#include <tiny_base/memorypool.h>
#include <tiny_core/protocol.h>
#include <tiny_http/http_model_fcgi.h>
#include <tiny_http/http_model_file.h>
#include <tiny_http/http_parser.h>
#include <tiny_http/http_responser.h>

#include <functional>
#include <memory>

// Using for web server.
class WebProtocol : public Protocol {
private:
    MemoryPool m_nPool;

    bool m_nKeepAlive;

    const unsigned int m_nBufferSize;
    const unsigned long m_nMaxChainSize;
    chain_t* m_pFileChain;
    chain_t* m_pWriteChain;

    bool m_nBeginSendHeader;
    bool m_nBeginSendFile;
    bool m_nUseWriteCompleteCallback;
    std::function<int(void)> m_fWriteRestFile;

    HttpParserSettings m_nSettings;
    HttpParser m_nParser;
    HttpBuilder m_nResponser;

    std::shared_ptr<HttpRequest> m_pRequest;
    std::shared_ptr<HttpResponse> m_pResponse;

public:
    WebProtocol();
    virtual void connectionMade();
    virtual void dataReceived(const std::string&);
    virtual void writeCompletely();
    virtual void connectionLost();
    
    void getDataFromProxy();

    virtual ~WebProtocol();
};

// Using for fcgi client.
class FcgiClientProtocol : public Protocol {
private:
    MemoryPool m_nPool;

    bool m_nKeepAlive;

    http_header m_nRequestHeader;
    HttpModelFcgi fcgiModel;

public:
    FcgiClientProtocol();
    void setFcgiRequest(http_header header)
    {
        m_nRequestHeader = header;
    }
    virtual void connectionMade();
    virtual void dataReceived(const std::string&);
    virtual void connectionLost();
    virtual ~FcgiClientProtocol();
};

// Using for new fcgi client.
class NewFcgiClientProtocol : public Protocol {
private:
    MemoryPool m_nPool;

    bool m_nKeepAlive;

    http_header m_nRequestHeader;
    HttpModelFcgi fcgiModel;

public:
    NewFcgiClientProtocol();
    void setFcgiRequest(http_header header)
    {
        m_nRequestHeader = header;
    }
    virtual void connectionMade();
    virtual void dataReceived(const std::string&);
    virtual void connectionLost();
    virtual ~NewFcgiClientProtocol();
};

#endif // !HTTP_PROTOCOL_H
