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

#include <tiny_http/http_parser.h>


#include <iostream>
#include <vector>
#include <string>
#include <list>

using namespace std;

typedef struct testHeader
{
    string key;
    string value;
} testHeader;

bool compare(const testHeader &a, const testHeader &b)
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
        .str = "Host: 127.0.0.1:9999\r\n"
               "Connection: close\r\n"
               "Content-Type: text/xml; charset=utf-8\r\n"
               "Accept-Encoding: compress, gzip\r\n"
               "Cookie: $Version=1; Skin=new;\r\n"
               "If-Modified-Since: Sat, 29 Oct 2010 19:43:31 GMT\r\n"
               "Referer: http://www.zcmhi.com/archives/71.html\r\n"
               "Content-Length: 348\r\n"
               "Last-Modified: Fri, 20 Apr 2018 08:12:56 GMT\r\n"
               "\r\n",
        .valid = true,
        .headerNum = 9,
        .headers = {
            {"Host", "127.0.0.1:9999"},
            {"Connection", "close"},
            {"Content-Type", "text/xml; charset=utf-8"},
            {"Accept-Encoding", "compress, gzip"},
            {"Cookie", "$Version=1; Skin=new;"},
            {"If-Modified-Since", "Sat, 29 Oct 2010 19:43:31 GMT"},
            {"Referer", "http://www.zcmhi.com/archives/71.html"},
            {"Content-Length", "348"},
            {"Last-Modified", "Fri, 20 Apr 2018 08:12:56 GMT"},
        },
    },

    {
        .str = "Host: 127.0.0.1:9999\r\n"
               "Connection: close\r\n"
               "\r\n",
        .valid = true,
        .headerNum = 2,
        .headers = {
            {"Host", "127.0.0.1:9999"},
            {"Connection", "close"},
        },
    },

    {
        .str = "Connection: close\r\n"
               "\r\n",
        .valid = true,
        .headerNum = 1,
        .headers = {
            {"Connection", "close"},
        },
    },

    {
        .str = "Content-Type: text/xml; charset=utf-8\r\n"
               "\r\n",
        .valid = true,
        .headerNum = 1,
        .headers = {
            {"Content-Type", "text/xml; charset=utf-8"},
        },
    },

    {
        .str = "Accept-Encoding: compress, gzip\r\n"
               "\r\n",
        .valid = true,
        .headerNum = 1,
        .headers = {
            {"Accept-Encoding", "compress, gzip"},
        },
    },

    {
        .str = "Cookie: $Version=1; Skin=new;\r\n"
               "\r\n",
        .valid = true,
        .headerNum = 1,
        .headers = {
            {"Cookie", "$Version=1; Skin=new;"},
        },
    },

    {
        .str = "If-Modified-Since: Sat, 29 Oct 2010 19:43:31 GMT\r\n"
               "\r\n",
        .valid = true,
        .headerNum = 1,
        .headers = {
            {"If-Modified-Since", "Sat, 29 Oct 2010 19:43:31 GMT"},
        },
    },

    {
        .str = "Referer: http://www.zcmhi.com/archives/71.html\r\n"
               "\r\n",
        .valid = true,
        .headerNum = 1,
        .headers = {
            {"Referer", "http://www.zcmhi.com/archives/71.html"},
        },
    },

    {
        .str = "Content-Length: 348\r\n"
               "\r\n",
        .valid = true,
        .headerNum = 1,
        .headers = {
            {"Content-Length", "348"},
        },
    },

    {
        .str = "Last-Modified: Fri, 20 Apr 2018 08:12:56 GMT\r\n"
               "\r\n",
        .valid = true,
        .headerNum = 1,
        .headers = {
            {"Last-Modified", "Fri, 20 Apr 2018 08:12:56 GMT"},
        },
    },

    {
        .str = "Transfer-Encoding: chunked\r\n"
               "\r\n",
        .valid = true,
        .headerNum = 1,
        .headers = {
            {"Transfer-Encoding", "chunked"},
        },
    },

    {
        .str = "\r\n",
        .valid = true,
        .headerNum = 0,
        .headers = {
            {},
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
        .str = "Content-MD5: DheThrvjUs/c+FtbVv7Sbw\r\n"
               "\r\n",
        .valid = true,
        .headerNum = 1,
        .headers = {
            {"Content-MD5", "DheThrvjUs/c+FtbVv7Sbw"},
        }

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
               "Strict-Transport-Security: max-age=31536000\r\n"
               "\r\n",
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
               "Connection: keep-alive\r\n"
               "\r\n",
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
               "Cookie: _ga=GA1.2.2068106829.1526513886; _gid=GA1.2.1899421896.1528880409\r\n"
               "\r\n",
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
               "Host: www.zcmhi.com\r\n"
               "\r\n",

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

void testParseHeaderMeaning()
{
    vector<int> notPass;
    int alltest = 0;
    int passtest = 0;

    HttpParserSettings settings;

    int len = sizeof(headers) / sizeof(headers[0]);

    for (int i = 0; i < len; i++)
    {
        alltest ++;

        HttpParser parser(&settings);
        parser.setType(HTTP_TYPE_REQUEST);

        int begin = 0;
        HttpHeaders *result = new HttpHeaders;
        int tmp = parser.parseHeaders(headers[i].str,
                                     begin,
                                     strlen(headers[i].str),
                                     result);

        bool res = (tmp == -1) ? false : true;
        if (res == headers[i].valid)
        {
            parser.parseHeadersMeaning(result);
            //printHttpHeaders(result);
            passtest++;
        }
        for (auto t : result->generals)
            delete t;
        delete result;
    }

    std::cout << "[Parse HeaderMeaning Test] pass/all = " << passtest << "/" << alltest << std::endl;

    if (!notPass.empty())
    {
        cout << "not pass url index:\n";
        for (auto t : notPass)
            std::cout<< t << " ";
        std::cout << std::endl;
    }

};

int main()
{
    headerMeaningInit();
    testParseHeaderMeaning();
    return 0;
}
