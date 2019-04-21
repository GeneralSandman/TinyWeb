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

#ifndef HTTP_BUILDER_H
#define HTTP_BUILDER_H

#include <TinyWebConfig.h>
#include <tiny_base/file.h>
#include <tiny_base/memorypool.h>
#include <tiny_http/http.h>
#include <tiny_http/http_model_chunked.h>
#include <tiny_http/http_model_file.h>
#include <tiny_http/http_model_gzip.h>
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

enum content_encoding_type {
    content_no_t = 1,
    content_gzip_t,
    content_deflate_t,
};

enum transport_encoding_type {
    transport_no_t = 1,
    transport_content_length_t,
    transport_chunked_t,
};

class HttpBuilder {

    MemoryPool* m_pPool;
    HttpModelChunk m_nChunkModel;
    HttpModelGzip m_nGzipModel;

    bool m_nWriteTailChunk;

    unsigned int m_nType : 2; //Http request or response

public:
    HttpBuilder(MemoryPool* pool);

    void setType(enum HttpContentType type) {
        m_nType = type;
    }

    /* response */
    void buildResponse(const HttpRequest* req, bool valid_requ, HttpResponse* response);
    void lineToStr(const HttpResponseLine* line, sdstr* line_str);
    void headersToStr(HttpResponseHeaders* headers, sdstr* headers_str);
    void bodyToStr(const HttpFile* file, sdstr* body_str);

    chain_t* bodyToChain(HttpFile* file,
        chain_t* chain,
        enum content_encoding_type cont,
        enum transport_encoding_type trans);

    bool noMoreBody(HttpFile* file,
        chain_t* chain,
        enum content_encoding_type cont,
        enum transport_encoding_type trans);

    // Don't use this function now.
    void response(const HttpRequest* req, std::string& data);

    /* request */
    void buildRequest(const HttpRequest* req_client, HttpRequest* req_server);
    void lineToStr(const HttpRequest* req, sdstr* line_str);
    void requestHeadersToStr(const HttpHeaders* headers, sdstr* headers_str);
    void bodyToStr2(const HttpFile* file, sdstr* body_str);


    ~HttpBuilder();
};

#endif // !HTTP_BUILDER_H
