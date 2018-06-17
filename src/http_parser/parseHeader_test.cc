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
#include <list>

using namespace std;

typedef struct header
{
    string key;
    string value;
} header;

bool compare(const header &a, const header &b)
{
    return (a.key == b.key) && (a.value == b.value);
}

typedef struct testHeaders
{
    const char *str;
    bool valid;
    vector<char *> headers;
} testHeaders;

testHeaders headers[] = {
    {.str = "Host: 127.0.0.1:9999\r\n",
     .valid = true,
     .headers = (vector<char *>){"Host",
                                 "127.0.0.1:9999"}

    },

    {
        .str = "Host: 127.0.0.1:9999\r\n"
               "Connection: keep-alive\r\n"
               "Upgrade-Insecure-Requests: 1\r\n"
               "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/65.0.3325.162 Safari/537.36\r\n"
               "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\r\n"
               "Accept-Encoding: gzip, deflate, br\r\n"
               "Accept-Language: zh-CN,zh;q=0.9,en-US;q=0.8,en;q=0.7\r\n"
               "\r\n",
        .valid = true,
        .headers = {

        },
    },

    {
        .str = "",
        .valid = true,
        .headers = {

        },
    },

    {
        .str = "",
        .valid = true,
        .headers = {

        },
    },

    {
        .str = "",
        .valid = true,
        .headers = {

        },
    },

};

void testParseUrl()
{
    typedef pair<int, string> key;
    vector<key> notPass;

    HttpParserSettings settings;

    int alltest = 0;
    int passtest = 0;

    int len = sizeof(headers) / sizeof(headers[0]);

    // for (int i = 0; i < len; i++)
    // {
    //     alltest++;

    //     HttpParser parser(&settings);
    //     parser.setType(HTTP_REQUEST);

    //     int begin = 0;
    //     Url *result = new Url;
    // std::cout << i << ")url:" << urls[i].url << std::endl;
    // int tmp = parser.parseUrl(urls[i].url,
    //                           begin,
    //                           strlen(urls[i].url),
    //                           result);

    //     bool res = (tmp == -1) ? false : true;
    //     if (res == urls[i].valid)
    //     {
    //         std::cout << "pass test\n";
    //         passtest++;
    //     }
    //     else
    //     {
    //         std::cout << "not pass test!!!\n";
    //         string tmp(urls[i].url);
    //         notPass.push_back(key(i, tmp));
    //     }

    //     if (res)
    //     {
    //         std::cout << "url valid\n";
    //         printUrl(result);
    //         std::cout << std::endl;
    //     }
    //     else
    //     {
    //         std::cout << "url invalid\n";
    //     }

    //     std::cout << std::endl;

    //     delete result;
    // }

    std::cout << passtest << "/" << alltest << std::endl;

    if (!notPass.empty())
    {
        cout << "not pass test urls:\n";
        for (auto t : notPass)
            std::cout << t.first << ")" << t.second << std::endl;
    }
    //We can't judge this urls is valid or invalid,
    //because '@' , IPv6 , UserInfo , port make judgement
    //become complicate.
    //So, we need to using api HttpParser::parseHost()
    //to solve this problem.
    //If you want get more information about host,
    //place view parseHost_test.cc file.
    //Good Luck, ^_^_^_^_^_^
};

int main()
{
    testParseUrl();
    // testParseUrl_();
    return 0;
}