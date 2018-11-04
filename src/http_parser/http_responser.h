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

#ifndef HTTP_RESPONSER_H
#define HTTP_REsPONSER_H

#include"http.h"
#include"http_parser.h"

#include<iostream>
#include<list>
#include<string.h>

void specialResponseBody(enum http_status s, std::string &res);

typedef struct HttpResponseHeaders
{
    std::list<HttpHeader *> generals;
}HttpResponseHeaders;

typedef struct HttpResponse
{
    HttpContentType type;

    unsigned short method : 8;
    unsigned short http_version_major : 8;
    unsigned short http_version_minor : 8;

    enum http_status status;
    unsigned int statusCode;
    Str statusPhrase;

    std::string method_s;

    unsigned short headerNum;

    bool special_response;

    File file;

    Url *url;
    HttpHeaders *headers;
    HttpBody *body;

}HttpResponse;

class HttpResponser
{

    public:
        HttpResponser()
        {
            std::cout << "class HttpResponser constructor\n";
        }

        int writeResponseLine()

        {

        }

        int writeHeader()
        {

        }

        int writeBody()
        {

        }

        int writeResponse()
        {
            writeResponseLine();
            writeHeader();
            writeBody();
        }

        ~HttpResponser()
        {
            std::cout << "class HttpResponser destructor\n";
        }
};

#endif
