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

#include"../http_parser.h"

#include <iostream>

using namespace std;

typedef struct testResponseLine
{
    const char * str;
    const bool valid;
    const bool isResponse;
    const int httpVersion;
    const char * statusCode;
    const char * statusPhrase;
}testResponseLine;

bool sameUrl(const Url * url, const char * str)
{
    return (0 == strncmp(url->data, str, url->len));
}

testResponseLine responseLines[] ={
    {
        .str = "HTTP/1.1 200 OK\r\n",
        .valid = true,
        .isResponse = true,
        .httpVersion = 11,
        .statusCode = "200",
        .statusPhrase = "OK",
    },

    {
        .str = "HTTP/1.0 304 Not Modified\r\n",
        .valid = true,
        .isResponse = true,
        .httpVersion = 10,
        .statusCode = "304",
        .statusPhrase = "Not Modified",
    },

    {
        .str = "HTTP/1.1 100 Continue\r\n",
        .valid = true,
        .isResponse = true,
        .httpVersion = 11,
        .statusCode = "100",
        .statusPhrase = "Continue",
    },

    {
        .str = "HTTP/1.1 201 Created\r\n",
        .valid = true,
        .isResponse = true,
        .httpVersion = 11,
        .statusCode = "201",
        .statusPhrase = "Created",
    },

    {
        .str = "HTTP/1.0 202 Accepted\r\n",
        .valid = true,
        .isResponse = true,
        .httpVersion = 10,
        .statusCode = "202",
        .statusPhrase = "Accepted",
    },
};

void testParseResponseLine()
{
    vector<int> notPass;

    HttpParserSettings settings;

    int alltest = 0;
    int passtest = 0;

    int len = sizeof(responseLines) / sizeof(responseLines[0]);

    for (int i = 0; i < len; i++)
    {
        std::cout << i << ")" << std::endl;
        alltest++;

        HttpParser parser(&settings);
        parser.setType(HTTP_TYPE_RESPONSE);

        int begin = 0;
        int len = strlen(responseLines[i].str);
        HttpRequest *request = new HttpRequest;

        int tmp = parser.execute(responseLines[i].str, begin, len, request);

        bool res = (tmp == -1) ? false : true;
        if (res == responseLines[i].valid)
        {
            if(res)
            {
                int version = request->http_version_major*10 + request->http_version_minor;
                bool sameversion = (version == responseLines[i].httpVersion);

                int codelen = strlen(responseLines[i].statusCode);
                int phraselen = strlen(responseLines[i].statusPhrase);

                std::cout<<"codelen:"<<codelen<<" pharselen:"<<phraselen<<std::endl;

                bool sameCode = (0 == strncmp(responseLines[i].statusCode, request->statusCode.data, codelen));
                bool samePhrase = (0 == strncmp(responseLines[i].statusPhrase, request->statusPhrase.data, phraselen));
                
                if (sameversion && sameCode && samePhrase)
                {
                    std::cout<<"++pass test"<<std::endl;
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
            std::cout<<"no match"<<std::endl;
        }

        delete request;
    }

    std::cout << passtest << "/" << alltest << std::endl;

    if (!notPass.empty())
    {
        for (auto t : notPass)
            std::cout << responseLines[t].str << " " << std::endl;
    }
};

int main()
{
    testParseResponseLine();
    return 0;
}
