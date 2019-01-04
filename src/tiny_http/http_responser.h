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

#include <TinyWebConfig.h>
#include <tiny_http/http.h>
#include <tiny_http/http_model_file.h>
#include <tiny_http/http_parser.h>

#include <iostream>
#include <list>
#include <stdlib.h>
#include <string.h>

void specialResponseBody(enum http_status s, std::string& res);

typedef struct HttpResponseLine {

    unsigned int http_version_major : 8;
    unsigned int http_version_minor : 8;

    enum http_status status;

} HttpResponseLine;

typedef struct HttpResponseHeaders {
    std::list<HttpHeader*> generals;

    std::string file_type;

    unsigned int valid_content_length : 1;

    unsigned int connection_keep_alive : 1;
    unsigned int connection_close : 1;
    unsigned int connection_upgrade : 1;

    unsigned int chunked : 1;

    unsigned int server : 1;

    unsigned int content_length_n;
} HttpResponseHeaders;

typedef struct HttpResponse {
    HttpResponseLine line;
    HttpResponseHeaders headers;
    HttpFile file;

} HttpResponse;

class HttpResponser {

public:
    HttpResponser();

    void responseLineToStr(const HttpResponseLine* line, sdstr* line_str);
    void responseHeadersToStr(HttpResponseHeaders* headers, sdstr* res);

    void buildResponse(const HttpRequest* req, HttpResponse* response);
    void response(const HttpRequest* req, std::string& data);

    ~HttpResponser();
};

#endif
