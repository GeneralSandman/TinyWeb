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

#include "http_responser.h"
#include "http_parser.h"
#include "http_model_file.h"

#include <iostream>
#include <string>

using namespace std;

#define TINYWEB_VERSION "TinyWeb/0.0.1 (ubuntu)"

void deheader(const HttpHeader * header, std::string &res)
{

    res += std::string(header->key.data, header->key.len);
    res += ": ";
    res += std::string(header->value.data, header->value.len);
}

typedef struct HttpResponseLine_testing
{

    unsigned int http_version_major : 8;
    unsigned int http_version_minor : 8;

    enum http_status status;

}HttpResponseLine_testing;

typedef struct HttpHeaders_testing
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
}HttpHeaders_testing;


void responseLineToStr(const HttpResponseLine_testing *line, std::string &line_str)
{
    line_str = "HTTP/";

    line_str += char(line->http_version_major + '0');
    line_str += '.'; 
    line_str += char(line->http_version_minor + '0');
    line_str += " ";

    line_str += std::string(httpStatusCode(line->status));
    line_str += " ";
    line_str += std::string(httpStatusStr(line->status));
    line_str += "\r\n";

}

void responseHeadersToStr(HttpHeaders_testing *headers, std::string &res)
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
        setStr(&(h2->value), "1000");
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
        setStr(&(h4->value), "text/html");
        headers->generals.push_back(h4);
    }

    std::string result;
    for(auto t: headers->generals)
    {
        //printf("%.*s->%.*s\n", t->key.len, t->key.data,
        //        t->value.len, t->value.data);
        deheader(t, res);
        res += "\r\n";
    }
    res += "\r\n";
}

void testDeheaders()
{

    HttpResponseLine_testing line;
    line.http_version_major = 1;
    line.http_version_minor = 1;
    line.status = HTTP_STATUS_FORBIDDEN;
    std::string line_str;
    responseLineToStr(&line, line_str);


    HttpHeaders_testing headers;
    headers.file_type = "text/html";
    headers.valid_content_length = 1;
    headers.connection_keep_alive = 1;
    headers.chunked = 1;
    headers.server = 1;
    std::string res;
    responseHeadersToStr(&headers, res);

    std::cout << line_str << res << std::endl;

    for (auto i = headers.generals.begin(); i != headers.generals.end(); i++)
    {
        delete (*i);
        *i = nullptr;
    }
}



void testGetMimeType(void)
{

    std::vector<std::string> fileNames;
    fileNames.push_back("dd.html");
    fileNames.push_back("dd/dd");
    fileNames.push_back("dd/1.jpeg");
    fileNames.push_back("dd/1.js");
    fileNames.push_back("dddd/ds.css");

    for (auto f: fileNames)
    {
        std::cout << f << " " << getType(f) << " " << getMimeType(getType(f)) << std::endl;
    }
}

int main()
{
    //testDeheaders();
    testGetMimeType();
    return 0;
}
