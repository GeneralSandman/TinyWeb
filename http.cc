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

#include "http.h"

#include <iostream>

//request
void printHttpRequestLine(const struct HttpRequestLine &line)
{
    std::cout << "HttpRequestLine:\n"
              << "\tmethod:" << line.method << "\n"
              << "\turl:" << line.url << "\n"
              << "\tversion:" << line.version << "\n";
}

void printHttpRequestHeader(const struct HttpRequestHeader &header)
{
    std::cout << "HttpRequestHeader:\n"
              << "\thost:" << header.host << "\n"
              << "\tconnection:" << header.connection << "\n"
              << "\tuser_agent:" << header.user_agent << "\n"
              << "\taccept:" << header.accept << "\n"
              << "\tcookie:" << header.cookie << "\n";
}

void printHttpRequestEntiyBody(const struct HttpRequestEntiyBody &body)
{
    std::cout << "HttpRequestEntiyBody:\n"
              << "\tbody:" << body.text << "\n";
}

void printHttpRequest(const struct HttpRequest &request)
{
    std::cout << "<--------HttpRequest------------>" << std::endl;
    printHttpRequestLine(request.line);
    printHttpRequestHeader(request.header);
    printHttpRequestEntiyBody(request.body);
    std::cout << "<------------------------------->" << std::endl;
}

//response
void printHttpResponseStatusLine(const struct HttpResponseStatusLine &line)
{
    std::cout << "HttpResponseStatusLine:\n"
              << "\tversion:" << line.version << "\n"
              << "\tstatusCode:" << line.statusCode << "\n"
              << "\tstatus:" << line.status << "\n";
}

void printHttpResponseHeader(const struct HttpResponseHeader &header)
{
    std::cout << "HttpResponseHeader:\n"
              << "\tdate:" << header.date << "\n"
              << "\tserver:" << header.server << "\n"
              << "\tlastModified:" << header.lastModified << "\n"
              << "\tcontentLength:" << header.contentLength << "\n"
              << "\tcontentType:" << header.contentType << "\n";
}

void printHttpResponseEntiyBody(const struct HttpResponseEntiyBody &body)
{
    std::cout << "HttpResponseEntiyBody:\n"
              << "\tbody:" << body.text << "\n";
}

void printHttpResponse(const struct HttpResponse &) {}