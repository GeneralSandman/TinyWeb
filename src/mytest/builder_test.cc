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

#include <tiny_base/memorypool.h>
#include <tiny_base/configer.h>
#include <tiny_http/http_parser.h>
#include <tiny_http/http_responser.h>
#include <tiny_struct/sdstr_t.h>

#include <iostream>
#include <vector>

using namespace std;

typedef struct testBuilder_t {
    const char* str;
    bool valid;
    const char* body;

} testBuilder_t;

testBuilder_t builders[] = {

    // 0
    {
        .str = "GET /1-63k_files/1k.txt HTTP/1.1\r\n"
               "Host: www.dissigil.cn\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /1-63k_files/2k.txt HTTP/1.1\r\n"
               "Host: www.dissigil.cn\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /1-63k_files/3k.txt HTTP/1.1\r\n"
               "Host: www.dissigil.cn\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /1-63k_files/4k.txt HTTP/1.1\r\n"
               "Host: www.dissigil.cn\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /1-63k_files/5k.txt HTTP/1.1\r\n"
               "Host: www.dissigil.cn\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    // 5
    {
        .str = "GET /1-63k_files/6k.txt HTTP/1.1\r\n"
               "Host: www.dissigil.cn\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /1-63k_files/7k.txt HTTP/1.1\r\n"
               "Host: www.dissigil.cn\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /1-63k_files/8k.txt HTTP/1.1\r\n"
               "Host: www.dissigil.cn\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /1-63k_files/9k.txt HTTP/1.1\r\n"
               "Host: www.dissigil.cn\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /1-63k_files/10k.txt HTTP/1.1\r\n"
               "Host: www.dissigil.cn\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /1-63k_files/11k.txt HTTP/1.1\r\n"
               "Host: www.dissigil.cn\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /1-63k_files/12k.txt HTTP/1.1\r\n"
               "Host: www.dissigil.cn\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /1-63k_files/13k.txt HTTP/1.1\r\n"
               "Host: www.dissigil.cn\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /1-63k_files/14k.txt HTTP/1.1\r\n"
               "Host: www.dissigil.cn\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /1-63k_files/15k.txt HTTP/1.1\r\n"
               "Host: www.dissigil.cn\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /1-63k_files/16k.txt HTTP/1.1\r\n"
               "Host: www.dissigil.cn\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /1-63k_files/17k.txt HTTP/1.1\r\n"
               "Host: www.dissigil.cn\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /1-63k_files/18k.txt HTTP/1.1\r\n"
               "Host: www.dissigil.cn\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

};

void testBuilder()
{
    HttpParserSettings settings;

    vector<int> notPass;
    int alltest = 0;
    int passtest = 0;

    int len = sizeof(builders) / sizeof(builders[0]);

    for (int i = 0; i < len; i++) {
        std::cout << i << ")\n";
        alltest++;

        int tmp = 0;
        int begin = 0;

        MemoryPool pool;
        HttpParser parser(&settings);
        parser.setType(HTTP_TYPE_REQUEST);
        HttpBuilder builder(&pool);
        HttpRequest* request_client = new HttpRequest;
        HttpRequest* request_server = new HttpRequest;
        request_server->headers = new HttpHeaders;
        httpHeadersInit(request_server->headers);

        tmp = parser.execute(builders[i].str,
            begin,
            strlen(builders[i].str),
            request_client);

        bool res = (tmp == -1) ? false : true;
        if (res) {
            std::cout << "parser success\n";
            passtest++;

            sdstr proxy_request_str;
            sdsnewempty(&proxy_request_str, 256);

            builder.buildRequest(request_client, request_server);
            builder.lineToStr(request_server, &proxy_request_str);
            builder.requestHeadersToStr(request_server->headers, &proxy_request_str);

            printf(&proxy_request_str);
            sdsdestory(&proxy_request_str);
        }

        delete request_server->headers;
        delete request_client;
        delete request_server;
    }

    std::cout << "[Build Http Request Test] pass/all = " << passtest << "/" << alltest << std::endl;
}

void initConfiger()
{
    std::string file = "../../TinyWeb.conf";
    Configer& configer = Configer::getConfigerInstance();
    configer.setConfigerFile(file);

    bool debug = true;
    if (0 == configer.loadConfig(debug))
        std::cout << "++load config successfull\n";
    else
        std::cout << "--load config failed\n";
    std::cout << std::endl;
}

int main()
{
    initConfiger();
    headerMeaningInit();

    testBuilder();
    return 0;
}
