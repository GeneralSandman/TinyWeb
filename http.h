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

/***************************************/
/*-----------Http Message--------------*/
/***************************************/
/*-----------HttpRequest--------------*/
/*RequestLine+RequestHeader+EntiyBody
/*

////////////////////////////////////////
/*-----------HttpResponse--------------*/
/*StatusLine+ResponseHeader+EntiyBody

/***************************************/

#ifndef HTTP_REQUEST_RESPONSE
#define HTTP_REQUEST_RESPONSE

#include <string>

//request
struct HttpRequestLine
{
    std::string method;
    std::string url;
    std::string version;
};

struct HttpRequestHeader
{
    std::string host;
    std::string connection;
    std::string user_agent;
    std::string accept;
    std::string cookie;
};

struct HttpRequestEntiyBody
{
    std::string text;
};

struct HttpRequest
{
    struct HttpRequestLine line;
    struct HttpRequestHeader header;
    struct HttpRequestEntiyBody body;
};

//response
struct HttpResponseStatusLine
{
    std::string version;
    std::string statusCode;
    std::string status;
};

struct HttpResponseHeader
{
    std::string date;
    std::string server;
    std::string lastModified;
    std::string contentLength;
    std::string contentType;
};

struct HttpResponseEntiyBody
{
    std::string text;
};

struct HttpResponse
{
    struct HttpResponseStatusLine line;
    struct HttpResponseHeader header;
    struct HttpResponseEntiyBody body;
};

void printHttpRequestLine(const struct HttpRequestLine &);
void printHttpRequestHeader(const struct HttpRequestHeader &);
void printHttpRequestEntiyBody(const struct HttpRequestEntiyBody &);
void printHttpRequest(const struct HttpRequest &);

void printHttpResponseStatusLine(const struct HttpResponseStatusLine &);
void printHttpResponseHeader(const struct HttpResponseHeader &);
void printHttpResponseEntiyBody(const struct HttpResponseEntiyBody &);
void printHttpResponse(const struct HttpResponse &);

#endif
