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
#include <boost/bind.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int getMessage()
{
    std::cout << "get Message\n";
}

int getUrl()
{
    cout << "get url\n";
}

int getStatus()
{
    cout << "get status\n";
}

int getRequestLine()
{
    cout << "get request line\n";
}

int getHeader()
{
    cout << "get request header\n";
}

int getHeaderKey()
{
    cout << "get header key\n";
}

int getHeaderValue()
{
    cout << "get header value\n";
}

int getBody()
{
    cout << "get body\n";
}

int endMessage()
{
    cout << "end message\n";
}

int getChunk()
{
    cout << "get chunk\n";
}

int endChunk()
{
    cout << "end chunk\n";
}

void testHttpParser()
{
    HttpParserSettings settings;
    settings.setGetMessageCallback(boost::bind(getMessage));
    settings.setGetRequestLineCallback(boost::bind(getRequestLine));
    settings.setGetHeaderCallback(boost::bind(getHeader));
    settings.setGetBodyCallback(boost::bind(getBody));
    settings.setGetEndMessageCallback(boost::bind(endMessage));

    HttpParser parser(&settings);

    parser.invokeByName("getMessage", nullptr, 0);
    parser.invokeByName("getRequestLine", nullptr, 0);
    parser.invokeByName("getHeader", nullptr, 0);
    parser.invokeByName("getBody", nullptr, 0);
    parser.invokeByName("endMessage", nullptr, 0);
}

void testHttpParserResponse()
{

    vector<string> responses = {
        "HTTP/1.1 200 OK\r\n"
        "Date: Tue, 04 Aug 2009 07:59:32 GMT\r\n"
        "Server: Apache\r\n"
        "X-Powered-By: Servlet/2.5 JSP/2.1\r\n"
        "Content-Type: text/xml; charset=utf-8\r\n"
        "Connection: close\r\n"
        "\r\n",

        "HTTP/1.1 304 Not Modified\r\n"
        "Server: Tengine\r\n"
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

    };

    HttpParserSettings settings;

    int len = responses.size();
    for (int i = 0; i < len; i++)
    {
        std::cout << "----" << i << std::endl;
        HttpParser parser(&settings);
        parser.setType(HTTP_RESPONSE);
        int begin = 0;
        // parser.execute(responses[i].c_str(), begin, responses[i].size());
    }
}

void testHttpParserRequest()
{

    vector<string> requests = {
        "GET http://127.0.0.1:9999/index.html HTTP/1.1\r\n"
        "Host: 127.0.0.1:9999\r\n"
        "Connection: keep-alive\r\n"
        "\r\n",

        "GET http://127.0.0.1:9999/index.html HTTP/1.1\r\n"
        "Host: www.baidu.com\r\n"
        "Connection: keep-alive\r\n"
        "Upgrade-Insecure-Requests: 1\r\n"
        "Accept: text/html\r\n"
        "Accept-Encoding: gzip, deflate, br\r\n"
        "Accept-Language: zh-CN,zh;q=0.9,en-US;q=0.8,en;q=0.7\r\n"
        "\r\n",

        "GET http://127.0.0.1:9999/index.html HTTP/1.1\r\n"
        "Host: www.dissigil.cn\r\n"
        "Connection: keep-alive\r\n"
        "Pragma: no-cache\r\n"
        "Cache-Control: no-cache\r\n"
        "Upgrade-Insecure-Requests: 1\r\n"
        "Accept-Encoding: gzip, deflate\r\n"
        "Accept-Language: zh-CN,zh;q=0.9,en-US;q=0.8,en;q=0.7\r\n"
        "Cookie: _ga=GA1.2.2068106829.1526513886; _gid=GA1.2.1899421896.1528880409\r\n"
        "\r\n",

    };

    HttpParserSettings settings;
    // settings.setGetMessageCallback(boost::bind(getMessage));
    // settings.setGetRequestLineCallback(boost::bind(getRequestLine));
    // settings.setGetHeaderCallback(boost::bind(getHeader));
    // settings.setGetBodyCallback(boost::bind(getBody));
    // settings.setGetEndMessageCallback(boost::bind(endMessage));

    int len = requests.size();
    for (int i = 0; i < 1; i++)
    {
        std::cout << i << ")" << std::endl;
        HttpParser parser(&settings);
        parser.setType(HTTP_REQUEST); //FIXME:
        HttpRequest *request = new HttpRequest;
        memset(request, 0, sizeof(HttpRequest));
        parser.execute(requests[i].c_str(), 0, requests[i].size(), request);

        if (request->url != nullptr)
            delete request->url;

        if (request->headers != nullptr)
            delete request->headers;

        if (request->body != nullptr)
            delete request->body;
        delete request;
    }
}

void testParseHeader()
{
    vector<string> strs = {
        "Host: 127.0.0.1:9999\r\n"
        "Connection: keep-alive\r\n"
        "\r\n",

        "Host: 127.0.0.1:9999\r\n"
        "Connection: keep-alive\r\n"
        "Upgrade-Insecure-Requests: 1\r\n"
        "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/65.0.3325.162 Safari/537.36\r\n"
        "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\r\n"
        "Accept-Encoding: gzip, deflate, br\r\n"
        "Accept-Language: zh-CN,zh;q=0.9,en-US;q=0.8,en;q=0.7\r\n"
        "\r\n",

        "sdfasd",

        "Connection: upgrade\r\n"
        ":authority: pingtas.qq.com\r\n"
        ":method: GET\r\n"
        ":path: /webview/pingd?dm=join.qq.com&pvi=3220461568&si=s2709209088&url=/apply.php&arg=&ty=1&rdm=&rurl=&rarg=&adt=&r2=49873873&r3=-1&r4=1&fl=&scr=1366x768&scl=24-bit&lg=zh-cn&jv=&tz=-8&ct=&ext=adid=&pf=&random=1528878932585\r\n"
        ":scheme: https\r\n"
        "\r\n",

        "Accept: text/plain, text/html\r\n"
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
    };

    HttpParserSettings settings;

    int len = strs.size();
    for (int i = 0; i < len; i++)
    {
        HttpParser parser(&settings);
        parser.setType(HTTP_REQUEST);
        int begin = 0;
        std::cout << "---" << i << "---\n";
        // parser.parseHeader(strs[i].c_str(), begin, strs[i].size());
    }
}

void testExecute()
{
    HttpParserSettings settings;
    settings.setGetMessageCallback(boost::bind(getMessage));
    settings.setGetRequestLineCallback(boost::bind(getRequestLine));
    settings.setGetHeaderCallback(boost::bind(getHeader));
    settings.setGetBodyCallback(boost::bind(getBody));
    settings.setGetEndMessageCallback(boost::bind(endMessage));

    HttpParser parser(&settings);
    parser.setType(HTTP_REQUEST);
}

void testHeaderKeyHash()
{
    vector<string> keys =
        {
            "Connection",
            "Content-Length",
            "Host",
            "Content-Type",
            "Accept-Encoding",
            "Cookie",
            "If-Modified-Since",
            "Referer",
            "Last-Modified",
            "Transfer-Encoding",
        };

    for (auto k : keys)
    {
        unsigned long long hash = 0;
        string key;
        for (int i = 0; i < k.size(); i++)
        {
            char ch = k[i];
            if (('a' <= ch && ch <= 'z') || ch == '-')
            {
            }
            else if (('A' <= ch && ch <= 'Z'))
            {
                ch += 32;
            }
            else
                ch = -1;
            if (ch == -1)
            {
                std::cout << "key is invalid\n";
                break;
            }

            key += ch;
            hash = getHash(hash, ch);
        }
        std::cout << key << ":" << hash << std::endl;
    }
}

void testGetMethod()
{
    int all = 0;
    int pass = 0;

    for (int i = 0; i < 34; i++)
    {
        const char *method = httpMethodStr((enum http_method)i);
        int len = strlen(method);

        if (i == (unsigned int)getMethod(method, len))
            pass++;
        else
        {
            std::cout << i << ":" << method << std::endl;
        }
        all++;
    }

    std::cout << pass << "/" << all << std::endl;
}

void testLitterCon()
{
    std::string tmp = "[]!@#$%^&*()_++=-:;'<>,./?\"`~";

    std::string str;

    for (int i = 0; i < 26; i++)
    {
        str += char(i + 'a');
        str += char(i + 'A');
    }
    str += tmp;

    std::string up;
    for (int i = 0; i < 26; i++)
    {
        up += char(i + 'A');
        up += char(i + 'A');
    }
    up += tmp;

    std::string lower;
    for (int i = 0; i < 26; i++)
    {
        lower += char(i + 'a');
        lower += char(i + 'a');
    }
    lower += tmp;

    std::string res1;
    for (auto t : str)
        res1 += toLower(t);

    std::string res2;
    for (auto t : str)
        res2 += toUpper(t);

    std::cout << res1 << std::endl
              << lower << std::endl
              << res2 << std::endl
              << up << std::endl;

    if (lower != res1 || up != res2)
        std::cout << "false\n";
    else
    {
        std::cout << "test pass\n";
    }
}

int main()
{
    // testHttpParser();
    // testHttpParserResponse();
    // testHttpParserRequest();
    // testParseHost();
    // testParseUrl();
    // testParseHeader();
    // testHeaderKeyHash();
    testGetMethod();
    // testLitterCon();
    return 0;
}
