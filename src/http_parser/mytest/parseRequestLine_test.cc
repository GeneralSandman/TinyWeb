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

#include"../my_http_parser.h"

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
    return (0 == strncmp(url->data+url->offset,str,url->len));
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
        .str = "GET / HTTP/1.1\r\n",
        .valid = true,
        .method = HTTP_METHOD_GET,
        .url = "/",
        .httpVersion = 11,
    },

    {
        .str = "GET / HTTP/1.1\r\n",
        .valid = true,
        .method = HTTP_METHOD_GET,
        .url = "/",
        .httpVersion = 11,
    },

};

void testParseRequestLine()
{
    vector<int> notPass;

    HttpParserSettings settings;

    int alltest = 0;
    int passtest = 0;

    int len = sizeof(requestLines) / sizeof(requestLines[0]);

    for (int i = 0; i < len; i++)
    {
        std::cout << i << ")" << std::endl;
        alltest++;

        HttpParser parser(&settings);
        parser.setType(HTTP_TYPE_REQUEST);

        int begin = 0;
        int len = strlen(requestLines[i].str);
        HttpRequest *request = new HttpRequest;

        int tmp = parser.execute(requestLines[i].str, begin, len, request);

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
                    std::cout<<"pass test"<<std::endl;
                    passtest++;
                }
                else
                {
                    notPass.push_back(i);
                }
            }
            else
            {
                std::cout<<"pass test"<<std::endl;
                passtest++;
            }
        }
        else
        {
            notPass.push_back(i);
        }

        delete request;
    }

    std::cout << passtest << "/" << alltest << std::endl;

    if (!notPass.empty())
    {
        for (auto t : notPass)
            std::cout << requestLines[t].str << " " << std::endl;
    }
};

int main()
{
    testParseRequestLine();
    return 0;
}
