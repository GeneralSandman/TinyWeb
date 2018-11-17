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
#include <TinyWebConfig.h>

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
            sdstr tmp;
            sdsnewempty(&tmp, 256); // more efficient
            if (headers->connection_keep_alive)
            {
                sdscat(&tmp, "Connection: keep-alive\r\n");
            }
            else if (headers->connection_close)
            {
                sdscat(&tmp, "Connection: close\r\n");
            }

            if (headers->valid_content_length)
            {
                sdscatsprintf(&tmp, "Content-Length: %u\r\n", headers->content_length_n);
            }

            if (headers->chunked)
            {
                sdscat(&tmp, "Transfer-Encoding: chunked\r\n");
            }

            if (headers->server)
            {
                char * version = TINYWEB_VERSION;
                sdscatsprintf(&tmp, "Server: %s\r\n", version);
            }

            if (headers->file_type.size())
            {
                sdscatsprintf(&tmp, "Content-Type: %s\r\n", headers->file_type.c_str());
            }

            sdscatsds(res, &tmp);
            sdscat(res, "\r\n");

            destory(&tmp);
        }

        void buildResponse(const HttpRequest *req, HttpResponse * response)
        {
            Url * url = req->url;
            sdstr wwwpath;
            sdsnewempty(&wwwpath);
            sdscat(&wwwpath, "/home/dell/TinyWeb/www");
            if (url->field_set & (1 << HTTP_UF_PATH))
            {
                unsigned int off = url->fields[HTTP_UF_PATH].offset;
                unsigned int len = url->fields[HTTP_UF_PATH].len;
                sdsncat(&wwwpath, url->data + off, len);
            }
            else
            {
                sdscat(&wwwpath, "/index.html");
            }
            std::cout << "file=";
            printf(&wwwpath);

            HttpResponseLine *line = &(response->line);
            line->http_version_major = req->http_version_major;
            line->http_version_minor = req->http_version_minor;
            line->status = HTTP_STATUS_OK;

            HttpResponseHeaders *headers = &(response->headers);
            headers->connection_keep_alive = 1;
            headers->chunked = 1;
            headers->server = 1;

            File *inputFile = &(response->file);
            std::string fname(wwwpath.data, wwwpath.len);// change to sdstr
            int return_val = initFile(inputFile, fname);
            if(return_val < 0)
            {
                std::cout << return_val << std::endl;
                return ;
            }

            headers->file_type = response->file.mime_type;
            headers->valid_content_length = 1;
            headers->content_length_n = response->file.info.st_size;

            destory(&wwwpath);
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
            // sendfile(0, &(resp->file));

            destory(&result);
            destoryFile(&(resp->file));
        }

        ~HttpResponser()
        {
            std::cout << "class HttpResponser destructor\n";
        }
};

#endif
