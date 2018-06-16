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
    bool valid;
    const char *host;
    const char *path;
} testUrl;

testUrl urls[] = {

    {.url = "https://www.dissigil.cn",
     .valid = true,
     .host = "www.dissigil.cn",
     .path = ""},

    {.url = "https://www.dissigil.cn/index.html",
     .valid = true,
     .host = "www.dissigil.cn",
     .path = "/index.html"},

    {.url = "http://dissigil.cn/",
     .valid = true,
     .host = "dissigil.cn",
     .path = "/"},

    {.url = "http://www.dissigil.cn:80",
     .valid = true,
     .host = "www.dissigil.cn:80",
     .path = ""},

    {.url = "http://www.dissigil.cn:8080/",
     .valid = true,
     .host = "www.dissigil.cn:8080",
     .path = "/"},

    {.url = "http://www.dissigil.cn:8080/index/index.html",
     .valid = true,
     .host = "www.dissigil.cn:8080",
     .path = "/index/index.html"},

    {.url = "www.dissigil.cn",
     .valid = false,
     .host = "",
     .path = ""},

    {.url = "www.dissigil.cn/index.html",
     .valid = false,
     .host = "",
     .path = ""},

    {.url = "www.dissigil.cn:80",
     .valid = false,
     .host = "",
     .path = ""},

    {.url = "www.dissigil.cn:80/index.html",
     .valid = false,
     .host = "",
     .path = ""},

    {.url = "www.dissigil.cn/home/index.html",
     .valid = false,
     .host = "",
     .path = ""},

    {.url = "http/www.dissigil.cn",
     .valid = false,
     .host = "",
     .path = ""},

    {.url = "http//www.dissigil.cn",
     .valid = false,
     .host = "",
     .path = ""},

    {.url = "http:www.dissigil.cn",
     .valid = false,
     .host = "",
     .path = ""},

    {.url = "http:/www.dissigil.cn",
     .valid = false,
     .host = "",
     .path = ""},

    {.url = "http:://www.dissigil.cn",
     .valid = false,
     .host = "",
     .path = ""},

    {.url = "/index.html",
     .valid = true,
     .host = "",
     .path = "/index.html"},

    {.url = "http://127.0.0.1:9999/",
     .valid = true,
     .host = "127.0.0.1:9999",
     .path = "/"},

    {.url = "http://127.0.0.1/",
     .valid = true,
     .host = "127.0.0.1",
     .path = "/"},

    {.url = "/asdf/adsd/sdf.html",
     .valid = true,
     .host = "",
     .path = "/asdf/adsd/sdf.html"},

    {.url = "http://a:b@host.com:8080/p/a/t/h?query=string#hash",
     .valid = true,
     .host = "a:b@host.com:8080",
     .path = "/p/a/t/h"},

    {.url = "http://hostname:80/home?query=li#head",
     .valid = true,
     .host = "hostname:80",
     .path = "/home"},

    {.url = "http://hostname:80/home#?query=li#head",
     .valid = true,
     .host = "hostname:80",
     .path = "/home"},

    {.url = "http://hostname:80/home??query=li##head",
     .valid = true,
     .host = "hostname:80",
     .path = "/home"},

    {.url = "http://hostname:80/home?abcd=abcd?query=li##head",
     .valid = true,
     .host = "hostname:80",
     .path = "/home"},

    {.url = "http://hostname:80/hom#e?query=li#head",
     .valid = true,
     .host = "hostname:80",
     .path = "/hom"},

    {.url = "/home/index.html", //false
     .valid = true,
     .host = "",
     .path = ""},

    {.url = "http://foo boar/",
     .valid = false,
     .host = "",
     .path = ""},

    {.url = "http://foo\nboar/",
     .valid = false,
     .host = "",
     .path = ""},

    {.url = "http://foo\rboar/",
     .valid = false,
     .host = "",
     .path = ""},

    {.url = "http://hostname/",
     .valid = true,
     .host = "hostname",
     .path = "/"},

    {.url = "http://hostname:444/",
     .valid = true,
     .host = "hostname:444",
     .path = "/"},

    {.url = "hostname:443",
     .valid = false,
     .host = "",
     .path = ""},

    {.url = "http://[1:2::3:4]/",
     .valid = true,
     .host = "1:2::3:4",
     .path = "/"},

    {.url = "http://[1:2::3:4]:67/",
     .valid = true,
     .host = "[1:2::3:4]:67",
     .path = "/"},

    {.url = "http://[1:2::3:4]:67/index/hom#e?query=li#head",
     .valid = true,
     .host = "[1:2::3:4]:67",
     .path = "/index/hom"},

    {.url = "http://[2001:0000:0000:0000:0000:0000:1.9.1.1]/",
     .valid = true,
     .host = "[2001:0000:0000:0000:0000:0000:1.9.1.1]",
     .path = "/"},

    {.url = "http://a.tbcdn.cn/p/fp/2010c/??fp-header-min.css,fp-base-min.css,"
            "fp-channel-min.css,fp-product-min.css,fp-mall-min.css,fp-category-min.css,"
            "fp-sub-min.css,fp-gdp4p-min.css,fp-css3-min.css,fp-misc-min.css?t=20101022.css",
     .valid = true,
     .host = "a.tbcdn.cn",
     .path = "/p/fp/2010c/"},

    {.url = "/toto.html?toto=a%20b",
     .valid = true,
     .host = "",
     .path = "/toto.html"},

    {.url = "/toto.html#titi",
     .valid = true,
     .host = "",
     .path = "/toto.html"},

    {.url = "http://www.webmasterworld.com/r.cgi?f=21&d=8405&url=",
     .valid = true,
     .host = "www.webmasterworld.com",
     .path = "/r.cgi"},

    {.url = "http://host.com:8080/p/a/t/h?query=string#hash",
     .valid = true,
     .host = "host.com:8080",
     .path = "/p/a/t/h"},

    {.url = "http://a:b@host.com:8080/p/a/t/h?query=string#hash",

     .valid = true,
     .host = "a:b@host.com:8080",
     .path = "/p/a/t/h"},

    {.url = "http://a:b@@hostname:443/",
     .valid = false,
     .host = "",
     .path = ""},

    {.url = "http://:443/",
     .valid = false,
     .host = "",
     .path = ""},

    {.url = "http://hostname:/",
     .valid = false,
     .host = "",
     .path = ""},

    {.url = "a:b@hostname:443",
     .valid = false,
     .host = "",
     .path = ""},

    {.url = ":443",
     .valid = false,
     .host = "",
     .path = ""},

    {.url = "hostname:",
     .valid = false,
     .host = "",
     .path = ""},

    {.url = "hostname:443/",
     .valid = false,
     .host = "",
     .path = ""},

    {.url = "/foo bar/",
     .valid = false,
     .host = "",
     .path = ""},

    {.url = "http://a%20:b@host.com/",
     .valid = true,
     .host = "a%20:b@host.com",
     .path = "/"},

    {.url = "/foo\rbar/",
     .valid = false,
     .host = "",
     .path = ""},

    {.url = "http://hostname::443/",
     .valid = false,
     .host = "",
     .path = ""},

    {.url = "http://a::b@host.com/",
     .valid = true,
     .host = "a::b@host.com",
     .path = "/"},

    {.url = "/foo\nbar/",
     .valid = false,
     .host = "",
     .path = ""},

    {.url = "http://@hostname/fo",
     .valid = true,
     .host = "hostname",
     .path = "/fo"},

    {.url = "http://host\name/fo",
     .valid = false,
     .host = "",
     .path = ""},

    {.url = "http://host%name/fo",
     .valid = false,
     .host = "",
     .path = ""},

    {.url = "http://host;ame/fo",
     .valid = false,
     .host = "",
     .path = ""},

    {.url = "http://a!;-_!=+$@host.com/",
     .valid = true,
     .host = "host.com",
     .path = "/"},

    {.url = "http://@/fo",
     .valid = false,
     .host = "",
     .path = ""},

    {.url = "http://@jljljl/fo",
     .valid = false,
     .host = "",
     .path = ""},

    {.url = "http://toto@/fo",
     .valid = false,
     .host = "",
     .path = ""},

    {.url = "http:///fo",
     .valid = false,
     .host = "",
     .path = ""},

    {.url = "http://host=ame/fo",
     .valid = false,
     .host = "",
     .path = ""},

    {.url = "http://[fe80::a%25eth0]/",
     .valid = true,
     .host = "fe80::a%25eth0",
     .path = "/"},

    {.url = "http://[fe80::a%eth0]/",
     .valid = true,
     .host = "fe80::a%eth0",
     .path = "/"},

    {.url = "http://[fe80::a%]/",
     .valid = false,
     .host = "",
     .path = ""},

    {.url = "http://[fe80::a%$HOME]/",
     .valid = false,
     .host = "",
     .path = ""},

    {.url = "http://[%eth0]/",
     .valid = false,
     .host = "",
     .path = ""},

    {.url = "/foo\tbar/",
     .valid = false,
     .host = "",
     .path = ""},

    {.url = "/foo\fbar/",
     .valid = false,
     .host = "",
     .path = ""},

    {.url = "https://www.dissigil.cn/home#sd#qu?query=li",
     .valid = true,
     .host = "www.dissigil.cn",
     .path = "/home"},

};

void testParseUrl()
{

    vector<string> notPass;

    HttpParserSettings settings;

    int alltest = 0;
    int passtest = 0;

    int len = sizeof(urls) / sizeof(urls[0]);

    for (int i = 0; i < len; i++)
    {
        alltest++;

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
        if (res == urls[i].valid)
        {
            std::cout << "pass test\n";
            passtest++;
        }
        else
        {
            std::cout << "not pass test!!!\n";
            string tmp(urls[i].url);
            notPass.push_back(tmp);
        }

        if (res)
        {
            std::cout << "url valid\n";
            printUrl(result);
            std::cout << std::endl;
        }
        else
        {
            std::cout << "url invalid\n";
        }

        std::cout << std::endl;

        delete result;
    }

    std::cout << passtest << "/" << alltest << std::endl;

    if (!notPass.empty())
    {
        cout << "not pass test urls:\n";
        for (auto t : notPass)
            std::cout << t << std::endl;
    }
    //We can't judge this urls is valid or invalid,
    //because '@' , IPv6 , UserInfo , port make judgement
    //become complicate.
    //So, we need to using api HttpParser::parseHost()
    //to solve this problem.
    //If you want get more information about host,
    //place view parseHost_test.cc file.
    //Good Luck, ^_^_^_^_^_^
}

int main()
{
    testParseUrl();
    // testParseUrl_();
    return 0;
}