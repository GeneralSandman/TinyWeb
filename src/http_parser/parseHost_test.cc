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

typedef struct testHost
{
    const char *host;
    bool hasAt;
    bool valid;
    const char *port;
} testHost;

testHost hosts[] = {
    {.host = "127.0.0.1",
     .hasAt = false,
     .valid = true,
     .port = ""},

    {.host = "255.0.0.0:80",
     .hasAt = false,
     .valid = true,
     .port = ""},

    {.host = "www.dissigil.cn:8080",
     .hasAt = false,
     .valid = true,
     .port = ""},

    {.host = "/www.dissigil.cn:80",
     .hasAt = false,
     .valid = true,
     .port = ""},

    {.host = "[1:2::3:4]",
     .hasAt = false,
     .valid = true,
     .port = ""},

    {.host = "[1:2::3:4]:80",
     .hasAt = false,
     .valid = true,
     .port = ""},

    {.host = "[2001:0000:0000:0000:0000:0000:1.9.1.1]",
     .hasAt = false,
     .valid = true,
     .port = ""},

    {.host = "a.tbcdn.cn:",
     .hasAt = false,
     .valid = true,
     .port = ""},

    {.host = "a:b@host.com:8080",
     .hasAt = true,
     .valid = true,
     .port = ""},

    {.host = "a:b@@hostname:443",
     .hasAt = true,
     .valid = true,
     .port = ""},

    {.host = "a:b@@hostname",
     .hasAt = true,
     .valid = true,
     .port = ""},

    {.host = "a%20:b@host.com",
     .hasAt = true,
     .valid = true,
     .port = ""},

    {.host = "@hostname",
     .hasAt = true,
     .valid = true,
     .port = ""},

    {.host = "http://www.dissigil.cn/",
     .hasAt = false,
     .valid = true,
     .port = ""},

    {.host = "foo\nbar",
     .hasAt = false,
     .valid = true,
     .port = ""},

    {.host = "foo\fbar",
     .hasAt = false,
     .valid = true,
     .port = ""},

    {.host = "foo\tbar",
     .hasAt = false,
     .valid = true,
     .port = ""},

};

void testParseUrl()
{

    HttpParserSettings settings;

    int all = sizeof(hosts) / sizeof(hosts[0]);
    int pass_test = 0;

    int len = sizeof(hosts) / sizeof(hosts[0]);
    for (int i = 0; i < len; i++)
    {
        HttpParser parser(&settings);
        parser.setType(HTTP_REQUEST);

        int begin = 0;
        Url *result = new Url;
        std::cout << i << ")host:" << hosts[i].host << std::endl;
        int tmp = parser.parseHost(hosts[i].host,
                                   begin,
                                   strlen(hosts[i].host),
                                   result,
                                   hosts[i].hasAt);

        bool res = (tmp == -1) ? false : true;
        if (res == hosts[i].valid)
        {
            std::cout << "pass test\n";
            pass_test++;
        }
        else
            std::cout << "not pass test\n";

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

    std::cout << pass_test << "/" << all << std::endl;
}

int main()
{
    testParseUrl();
    // testParseUrl_();
    return 0;
}