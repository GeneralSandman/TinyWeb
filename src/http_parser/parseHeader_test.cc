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

#define maxHeaders 23 * 2

typedef struct testHeaders
{
    const char *str;
    bool valid;
    int headerNum;
    char *headers[maxHeaders][2];
} testHeaders;

testHeaders headers[] = {
    {
        .str = "Host: 127.0.0.1:9999\r\n",
        .valid = true,
        .headerNum = 1,
        .headers = {
            {"Host", "127.0.0.1:9999"},
        },
    },

    {
        .str = "Date: Tue, 04 Aug 2009 07:59:32 GMT\r\n"
               "Server: Apache\r\n"
               "X-Powered-By: Servlet/2.5 JSP/2.1\r\n"
               "Content-Type: text/xml; charset=utf-8\r\n"
               "Connection: close\r\n"
               "\r\n",
        .valid = true,
        .headerNum = 5,
        .headers = {
            {"Date", "Tue, 04 Aug 2009 07:59:32 GMT"},
            {"Server", "Apache"},
            {"X-Powered-By", "Servlet/2.5 JSP/2.1"},
            {"Content-Type", "text/xml; charset=utf-8"},
            {"Connection", "close"},
        },
    },

    {
        .str = "Server: Tengine\r\n"
               "Date: Mon, 11 Jun 2018 07:18:14 GMT\r\n"
               "Content-Type: text/html; charset=utf-8\r\n"
               "Connection: keep-alive\r\n"
               "Vary: Ali-Detector-Type\r\n"
               "Cache-Control: max-age=60, s-maxage=300\r\n"
               "X-Snapshot-Age: 1\r\n"
               "Content-MD5: DheThrvjUs/c+FtbVv7Sbw==\r\n"
               "ETag: W/\"29d7-163e38938b3\"\r\n"
               "Via: cache9.l2cm12-1[0,304-0,H], cache48.l2cm12-1[0,0], cache6.cn295[0,304-0,H], cache6.cn295[1,0]\r\n"
               "Age: 121\r\n"
               "X-Cache: HIT TCP_IMS_HIT dirn:6:453356086 mlen:-1\r\n"
               "Timing-Allow-Origin: *\r\n"
               "EagleId: 7ceee81815287014946507756e\r\n"
               "Strict-Transport-Security: max-age=31536000\r\n",
        .valid = true,
        .headerNum = 15,
        .headers = {
            {"Server", "Tengine"},
            {"Date", "Mon, 11 Jun 2018 07:18:14 GMT"},
            {"Content-Type", "text/html; charset=utf-8"},
            {"Connection", "keep-alive"},
            {"Vary", "Ali-Detector-Type"},
            {"Cache-Control", "max-age=60, s-maxage=300"},
            {"X-Snapshot-Age", "1"},
            {"Content-MD5", "DheThrvjUs/c+FtbVv7Sbw=="},
            {"ETag", "W/\"29d7-163e38938b3\""},
            {"Via", "cache9.l2cm12-1[0,304-0,H], cache48.l2cm12-1[0,0], cache6.cn295[0,304-0,H], cache6.cn295[1,0]"},
            {"Age", "121"},
            {"X-Cache", "HIT TCP_IMS_HIT dirn:6:453356086 mlen:-1"},
            {"Timing-Allow-Origin", "*"},
            {"EagleId", "7ceee81815287014946507756e"},
            {"Strict-Transport-Security", "max-age=31536000"},
        },
    },

    {
        .str = "Host: 127.0.0.1:9999\r\n"
               "Connection: keep-alive\r\n",
        .valid = true,
        .headerNum = 2,
        .headers = {
            {"Host", "127.0.0.1:9999"},
            {"Connection", "keep-alive"},
        },
    },

    {
        .str = "Host: www.dissigil.cn\r\n"
               "Connection: keep-alive\r\n"
               "Pragma: no-cache\r\n"
               "Cache-Control: no-cache\r\n"
               "Upgrade-Insecure-Requests: 1\r\n"
               "Accept-Encoding: gzip, deflate\r\n"
               "Accept-Language: zh-CN,zh;q=0.9,en-US;q=0.8,en;q=0.7\r\n"
               "Cookie: _ga=GA1.2.2068106829.1526513886; _gid=GA1.2.1899421896.1528880409\r\n",
        .valid = true,
        .headerNum = 8,
        .headers = {
            {"Host", "www.dissigil.cn"},
            {"Connection", "keep-alive"},
            {"Pragma", "no-cache"},
            {"Cache-Control", "no-cache"},
            {"Upgrade-Insecure-Requests", "1"},
            {"Accept-Encoding", "gzip, deflate"},
            {"Accept-Language", "zh-CN,zh;q=0.9,en-US;q=0.8,en;q=0.7"},
            {"Cookie", "_ga=GA1.2.2068106829.1526513886; _gid=GA1.2.1899421896.1528880409"},
        },
    },

    {
        .str = "Accept: text/plain, text/html\r\n"
               "Accept-Charset: iso-8859-5\r\n"
               "Accept-Encoding: compress, gzip\r\n"
               "Accept-Language: en,zh\r\n"
               "Accept-Ranges: bytes\r\n"
               "Authorization: Basic QWxhZGRpbjpvcGVuIHNlc2FtZQ==\r\n"
               "Cache-Control: no-cache\r\n"
               "Connection: close\r\n"
               "Cookie: $Version=1; Skin=new;\r\n"
               "Content-Length: 348\r\n"
               "Content-Type: application/x-www-form-urlencoded\r\n"
               "Date: Tue, 15 Nov&nbsp;2010 08:12:31 GMT\r\n"
               "Expect: 100-continue\r\n"
               "From: user@email.com\r\n"
               "Host: www.zcmhi.com\r\n",

        .valid = true,
        .headerNum = 15,
        .headers = {
            {"Accept", "text/plain, text/html"},
            {"Accept-Charset", "iso-8859-5"},
            {"Accept-Encoding", "compress, gzip"},
            {"Accept-Language", "en,zh"},
            {"Accept-Ranges", "bytes"},
            {"Authorization", "Basic QWxhZGRpbjpvcGVuIHNlc2FtZQ=="},
            {"Cache-Control", "no-cache"},
            {"Connection", "close"},
            {"Cookie", "$Version=1; Skin=new;"},
            {"Content-Length", "348"},
            {"Content-Type", "application/x-www-form-urlencoded"},
            {"Date", "Tue, 15 Nov&nbsp;2010 08:12:31 GMT"},
            {"Expect", "100-continue"},
            {"From", "user@email.com"},
            {"Host", "www.zcmhi.com"},
        },
    },
};

void testParseHeader()
{
    vector<int> notPass;

    HttpParserSettings settings;

    int alltest = 0;
    int passtest = 0;

    int len = sizeof(headers) / sizeof(headers[0]);

    for (int i = 2; i < 3; i++)
    {
        alltest++;

        HttpParser parser(&settings);
        parser.setType(HTTP_REQUEST);

        int begin = 0;
        HttpHeaders *result = new HttpHeaders;
        int tmp = parser.parseHeader(headers[i].str,
                                     begin,
                                     strlen(headers[i].str),
                                     result);

        bool res = (tmp == -1) ? false : true;
        if (res == headers[i].valid)
        {
            if (res)
            {
                std::cout << "url valid\n";
                printHttpHeaders(result);

                int j = 0;
                int lenj = headers[i].headerNum;
                std::cout << lenj << std::endl;

                auto p = result->generals.begin();
                auto lenp = result->generals.end();

                while (j != lenj && p != lenp)
                {
                    // std::cout << headers[i].headers[j][0] << (*p)->key.c_str() << std::endl;
                    // std::cout << headers[i].headers[j][1] << (*p)->value.c_str() << std::endl;
                    if (0 != strcmp(headers[i].headers[j][0], (*p)->key.c_str()) ||
                        0 != strcmp(headers[i].headers[j][1], (*p)->value.c_str()))
                    {
                        std::cout << "-------------\n";
                        break;
                    }

                    j++;
                    p++;
                }

                if (j == lenj && p == lenp)
                    passtest++;
            }
        }
        else
        {
            std::cout << "not pass test!!!\n";
            printHttpHeaders(result);
            notPass.push_back(i);
        }
        for (auto t : result->generals)
            delete t;
        delete result;
    }

    std::cout << passtest << "/" << alltest << std::endl;

    if (!notPass.empty())
    {
        cout << "not pass test urls:\n";
        for (auto t : notPass)
            std::cout << t << " " << std::endl;
    }
};

int main()
{
    testParseHeader();
    return 0;
}