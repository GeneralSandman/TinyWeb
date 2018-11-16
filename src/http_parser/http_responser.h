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

#include <http_parser/http.h>
#include <http_parser/http_model_file.h>
#include <http_parser/http_parser.h>
#include <tiny_struct/sdstr_t.h>

#include <iostream>
#include <list>
#include <string.h>
#include <stdlib.h>

void specialResponseBody(enum http_status s, std::string &res);


typedef struct HttpResponseLine
{

    unsigned int http_version_major : 8;
    unsigned int http_version_minor : 8;

    enum http_status status;

}HttpResponseLine;

typedef struct HttpResponseHeaders
{
    std::list<HttpHeader *> generals;

    std::string file_type;

    unsigned int valid_content_length : 1;

    unsigned int connection_keep_alive : 1;
    unsigned int connection_close : 1;
    unsigned int connection_upgrade : 1;

    unsigned int chunked : 1;

    unsigned int server : 1;

    unsigned int content_length_n;
}HttpResponseHeaders;

typedef struct HttpResponse
{
    HttpResponseLine line;
    HttpResponseHeaders headers;
    File file;

}HttpResponse;


class HttpResponser
{

    public:
        HttpResponser()
        {
            std::cout << "class HttpResponser constructor\n";
        }

        void deheader(const HttpHeader * header, sdstr *res)
        {
            sdscatsprintf(res, "%.*s: %.*s", header->key.len, header->key.data,
                    header->value.len, header->value.data);
        }

        void responseLineToStr(const HttpResponseLine *line, sdstr *line_str)
        {
            unsigned int major = line->http_version_major;
            unsigned int minor = line->http_version_minor;
            const char *code = httpStatusCode(line->status);
            const char *phrase = httpStatusStr(line->status);
            
            sdscatsprintf(line_str, "HTTP/%d.%d %s %s\r\n", major, minor, code, phrase);
        }

        void responseHeadersToStr(HttpResponseHeaders *headers, sdstr *res)
        {
            if (headers->connection_keep_alive)
            {
                HttpHeader *h1 = new HttpHeader;
                setStr(&(h1->key), "Connection");
                setStr(&(h1->value), "keep-alive");
                headers->generals.push_back(h1);
            }
            else if (headers->connection_close)
            {
                HttpHeader *h1 = new HttpHeader;
                setStr(&(h1->key), "Connection");
                setStr(&(h1->value), "close");
                headers->generals.push_back(h1);
            }

            if (headers->valid_content_length)
            {
                HttpHeader *h2 = new HttpHeader;
                setStr(&(h2->key), "Content-Length");
                std::string len = std::to_string(headers->content_length_n);
                setStr(&(h2->value), len.c_str());
                headers->generals.push_back(h2);
            }

            if (headers->chunked)
            {
                HttpHeader *h3 = new HttpHeader;
                setStr(&(h3->key), "Transfer-Encoding");
                setStr(&(h3->value), "chunked");
                headers->generals.push_back(h3);
            }

            if (headers->server)
            {
                HttpHeader *h3 = new HttpHeader;
                setStr(&(h3->key), "Server");
                setStr(&(h3->value), "TinyWeb/0.0.1 (ubuntu)");
                headers->generals.push_back(h3);
            }

            if (headers->file_type.size())
            {
                HttpHeader *h4 = new HttpHeader;
                setStr(&(h4->key), "Content-Type");
                setStr(&(h4->value), headers->file_type.c_str());
                headers->generals.push_back(h4);
            }

            for(auto t: headers->generals)
            {
                deheader(t, res);
                sdscat(res, "\r\n");
            }
            sdscat(res, "\r\n");
        }

        void buildResponse(const HttpRequest *req, HttpResponse * response)
        {

            HttpResponseLine *line = &(response->line);
            line->http_version_major = req->http_version_major;
            line->http_version_minor = req->http_version_minor;
            line->status = HTTP_STATUS_OK;

            HttpResponseHeaders *headers = &(response->headers);
            headers->connection_keep_alive = 1;
            headers->chunked = 1;
            headers->server = 1;

            std::string fname = "input.html";
            File *inputFile = &(response->file);
            int return_val = initFile(inputFile, fname);
            if(return_val < 0)
            {
                std::cout << return_val << std::endl;
                return ;
            }

            headers->file_type = response->file.mime_type;
            headers->valid_content_length = 1;
            headers->content_length_n = response->file.info.st_size;
        }

        void response(const HttpRequest *req)
        {
            HttpResponse *resp = new HttpResponse;
            buildResponse(req, resp);

            sdstr result;
            sdsnewempty(&result);

            responseLineToStr(&(resp->line), &result);
            responseHeadersToStr(&(resp->headers), &result);

            printf(&result);
            sendfile(0, &(resp->file));

            destory(&result);
            destoryFile(&(resp->file));
        }

        ~HttpResponser()
        {
            std::cout << "class HttpResponser destructor\n";
        }
};

#endif
