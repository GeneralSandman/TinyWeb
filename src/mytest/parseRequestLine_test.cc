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

#include <http_parser/http_parser.h>

#include <iostream>

using namespace std;

typedef struct testRequestLine
{
    const char * str;
    const bool valid;
    enum http_method method;
    const char * url;
    const int httpVersion;
}testRequestLine;

bool sameUrl(const Url * url, const char * str)
{
    return (0 == strncmp(url->data,str,url->len));
}

testRequestLine requestLines[] ={
    {
        .str = "GET http://dissigil.cn/resume.pdf HTTP/1.1\r\n",
        .valid = true,
        .method = HTTP_METHOD_GET,
        .url = "http://dissigil.cn/resume.pdf",
        .httpVersion = 11,
    },

    {
        .str = "GET http://dissigil.cn/resume.pdf HTTP/1.0\r\n",
        .valid = true,
        .method = HTTP_METHOD_GET,
        .url = "http://dissigil.cn/resume.pdf",
        .httpVersion = 10,
    },

    {
        .str = "sss sss sss",
        .valid = false,
        .method = HTTP_METHOD_INVALID,
        .url = "",
        .httpVersion = 0,
    },

    {
        .str = "POST /index.html HTTP/1.1\r\n",
        .valid = true,
        .method = HTTP_METHOD_POST,
        .url = "/index.html",
        .httpVersion = 11,
    },

    {
        .str = "POST /index.html HTTP/1.0\r\n",
        .valid = true,
        .method = HTTP_METHOD_POST,
        .url = "/index.html",
        .httpVersion = 10,
    },

};

void testParseRequestLine()
{
    vector<int> notPass;
    int alltest = 0;
    int passtest = 0;

    HttpParserSettings settings;

    int len = sizeof(requestLines) / sizeof(requestLines[0]);

    for (int i = 0; i < len; i++)
    {
        alltest++;

        HttpParser parser(&settings);
        parser.setType(HTTP_TYPE_REQUEST);

        int begin = 0;
        int str_len = strlen(requestLines[i].str);
        HttpRequest *request = new HttpRequest;

        int tmp = parser.execute(requestLines[i].str, begin, str_len, request);

        bool res = (tmp == -1) ? false : true;
        if (res == requestLines[i].valid)
        {
            if(res)
            {
                int version = request->http_version_major*10 + request->http_version_minor;
                bool sameversion = (version == requestLines[i].httpVersion);
                bool sameurl = sameUrl(request->url, requestLines[i].url);
                bool samemethod = (request->method == requestLines[i].method);

                if (sameversion && sameurl && samemethod)
                {
                    passtest++;
                }
                else
                {
                    notPass.push_back(i);
                }
            }
            else
            {
                passtest++;
            }
        }
        else
        {
            notPass.push_back(i);
        }

        delete request;
    }

    std::cout << "[Parse ReqLine Test] pass/all = " << passtest << "/" << alltest << std::endl;

    if (!notPass.empty())
    {
        cout << "not pass req-line index:\n";
        for (auto t : notPass)
            std::cout << t << " ";
        std::cout << std::endl;
    }
};

int main()
{
    testParseRequestLine();
    return 0;
}
