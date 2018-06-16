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

#include "my_http_parser.h"

#include <iostream>
#include <vector>
#include <string>

using namespace std;

typedef struct testUrl
{
    const char *url;
    bool vaild;
    const char *host;
    const char *path;
} testUrl;

testUrl urls[] = {

    {.url = "https://www.dissigil.cn",
     .vaild = true,
     .host = "www.dissigil.cn",
     .path = ""},

    {.url = "https://www.dissigil.cn/index.html",
     .vaild = true,
     .host = "www.dissigil.cn",
     .path = "/index.html"},

    {.url = "http://dissigil.cn/",
     .vaild = true,
     .host = "dissigil.cn",
     .path = "/"},

    {.url = "http://www.dissigil.cn:80",
     .vaild = true,
     .host = "www.dissigil.cn:80",
     .path = ""},

    {.url = "http://www.dissigil.cn:8080/",
     .vaild = true,
     .host = "www.dissigil.cn:8080",
     .path = "/"},

    {.url = "http://www.dissigil.cn:8080/index/index.html",
     .vaild = true,
     .host = "www.dissigil.cn:8080",
     .path = "/index/index.html"},

    {.url = "www.dissigil.cn",
     .vaild = false,
     .host = nullptr,
     .path = nullptr},

    {.url = "www.dissigil.cn/index.html",
     .vaild = false,
     .host = nullptr,
     .path = nullptr},

    {.url = "www.dissigil.cn:80",
     .vaild = false,
     .host = nullptr,
     .path = nullptr},

    {.url = "www.dissigil.cn:80/index.html",
     .vaild = false,
     .host = nullptr,
     .path = nullptr},

    {.url = "www.dissigil.cn/home/index.html",
     .vaild = false,
     .host = nullptr,
     .path = nullptr},

    {.url = "http/www.dissigil.cn",
     .vaild = false,
     .host = nullptr,
     .path = nullptr},

    {.url = "http//www.dissigil.cn",
     .vaild = false,
     .host = nullptr,
     .path = nullptr},

    {.url = "http:www.dissigil.cn",
     .vaild = false,
     .host = nullptr,
     .path = nullptr},

    {.url = "http:/www.dissigil.cn",
     .vaild = false,
     .host = nullptr,
     .path = nullptr},

    {.url = "http:://www.dissigil.cn",
     .vaild = false,
     .host = nullptr,
     .path = nullptr},

    {.url = "/index.html",
     .vaild = true,
     .host = nullptr,
     .path = "/index.html"},

    {.url = "http://127.0.0.1:9999/",
     .vaild = true,
     .host = "127.0.0.1:9999",
     .path = "/"},

    {.url = "http://127.0.0.1/",
     .vaild = true,
     .host = "127.0.0.1",
     .path = "/"},

    {.url = "/asdf/adsd/sdf.html",
     .vaild = true,
     .host = "",
     .path = "/asdf/adsd/sdf.html"},

    {.url = "http://a:b@host.com:8080/p/a/t/h?query=string#hash",
     .vaild = true,
     .host = "a:b@host.com:8080",
     .path = "/p/a/t/h?query=string#hash"},

    {.url = "/home/index.html", //false
     .vaild = true,
     .host = nullptr,
     .path = nullptr},

    {.url = "http://foo boar/",
     .vaild = false,
     .host = nullptr,
     .path = nullptr},

    {.url =
         "http://foo\nboar/",
     .vaild = false,
     .host = nullptr,
     .path = nullptr},

    {.url =
         "http://foo\rboar/",
     .vaild = false,
     .host = nullptr,
     .path = nullptr},

};

void testParseUrl_()
{
    //TODO:
    //replace urls with urls.txt
    vector<string> urls = {
        // "https://blog.csdn.net/foruok/article/details/8954726",
        // "www.baidu.com",
        "www.dissigil.cn", //false
        "www.dissigil.cn/index.html",
        "www.dissigil.cn:80",
        "www.dissigil.cn:80/index.html",
        "www.dissigil.cn/home/artile/li.html",

        "https://www.dissigil.cn", //true
        "https://www.dissigil.cn/index.html",
        "https://www.dissigil.cn:80",
        "https://www.dissigil.cn:80/index.html",
        "https://www.dissigil.cn/home/artile/li.html",

        "http/www.dissigil.cn", //false
        "http//www.dissigil.cn",
        "http:www.dissigil.cn",
        "http:/www.dissigil.cn",
        "http:://www.dissigil.cn",

        "/index.html", //true
        "http://127.0.0.1:9999/",
        "http://[::]/index.html",
        "/asdf/adsd/sdf.html",
        "http://a:b@host.com:8080/p/a/t/h?query=string#hash",

        "//index.html", //false
        "http://foo boar/",
        "http://foo\nboar/",
        "http://foo\rboar/",
        "http://foo\tboar/"

    };

    HttpParserSettings settings;

    for (int i = 0; i < urls.size(); i++)
    {
        HttpParser parser(&settings);
        parser.setType(HTTP_REQUEST);
        int begin = 0;

        Url *result = new Url;
        std::cout << i << "->" << urls[i] << std::endl;
        // if (-1 == parser.parseUrl(urls[i], begin, urls[i].size(), result))
        {
            std::cout << "parse error\n"
                      << std::endl;
            continue;
        }
        // parser.parseHeader(urls[i], begin, urls[i].size());
        std::cout << "parse success" << std::endl;
        printUrl(result);
        std::cout << std::endl;
        delete result;
        // cout << "begin:" << begin << endl;
    }
}

void testParseUrl()
{

    HttpParserSettings settings;

    int all = sizeof(urls) / sizeof(urls[0]);
    int pass_test = 0;

    int len = sizeof(urls) / sizeof(urls[0]);
    for (int i = 0; i < len; i++)
    {
        HttpParser parser(&settings);
        parser.setType(HTTP_REQUEST);

        int begin = 0;
        Url *result = new Url;
        std::cout << i << ")url:" << urls[i].url << std::endl;
        int tmp = parser.parseUrl(urls[i].url,
                                  begin,
                                  strlen(urls[i].url),
                                  result);

        bool res = (tmp == -1) ? false : true;
        if (res == urls[i].vaild)
        {
            std::cout << "pass test\n";
            pass_test++;
        }
        else
            std::cout << "not pass test\n";

        if (res)
        {
            std::cout << "url vaild\n";
            printUrl(result);
            std::cout << std::endl;
        }
        else
        {
            std::cout << "url invaild\n";
        }

        std::cout << std::endl;

        delete result;
    }

    std::cout << pass_test << "/" << all << std::endl;
}

int main()
{
    testParseUrl();
    // testParseUrl_();
    return 0;
}