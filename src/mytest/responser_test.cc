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
#include <tiny_http/http_responser.h>

#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>

using namespace std;

typedef struct testBody
{
    const char * str;
    bool valid;
    const char * body;


}testBody;

testBody bodys[]=
{
    {
        .str = 
            "GET http://127.0.0.1:9999/welcome.html HTTP/1.1\r\n"
            "Transfer-Encoding: chunked\r\n"
            "\r\n"
            "25\r\n"
            "This is the data in the first chunk..\r\n"
            "1C\r\n"
            "and this is the second one..\r\n"
            "0\r\n"
            "\r\n",
        .valid = true,
        .body = "This is the data in the first chunk.."
            "and this is the second one..",
    },

    {
        .str = 
            "GET http://127.0.0.1:9999/ HTTP/1.1\r\n"
            "Transfer-Encoding: chunked\r\n"
            "\r\n"
            "25\r\n"
            "This is the data in the first chunk..\r\n"
            "1C\r\n"
            "and this is the second one..\r\n"
            "0\r\n"
            "\r\n",
        .valid = true,
        .body = "This is the data in the first chunk.."
            "and this is the second one..",
    },

    {
        .str = 
            "GET http://127.0.0.1:9999 HTTP/1.1\r\n"
            "Transfer-Encoding: chunked\r\n"
            "\r\n"
            "25\r\n"
            "This is the data in the first chunk..\r\n"
            "1C\r\n"
            "and this is the second one..\r\n"
            "0\r\n"
            "\r\n",
        .valid = true,
        .body = "This is the data in the first chunk.."
            "and this is the second one..",
    },

    {
        .str = 
            "GET http://127.0.0.1:9999/index.html HTTP/1.1\r\n"
            "Content-Length: 10\r\n"
            "\r\n"
            "helloworlddd",
        .valid = true,
        .body = "helloworld",
    },

    {
        .str = 
            "GET http://127.0.0.1:9999/index.html HTTP/1.1\r\n"
            "\r\n"
            "helloworld",
        .valid = true,
        .body = "helloworld",
    },

    {
        .str = 
            "GET http://127.0.0.1:9999/index.html HTTP/1.1\r\n"
            "Transfer-Encoding: chunked\r\n"
            "25\r\n"
            "This is the data in the first chunk\r\n"
            "\r\n"
            "1C\r\n"
            "and this is the second one\r\n"
            "\r\n"
            "0\r\n"
            "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = 
            "GET http://127.0.0.1:9999/index.html HTTP/1.1\r\n"
            "Transfer-Encoding: chunked\r\n"
            "25\r\n"
            "This is the data in the first chunk\r\n"
            "\r\n"
            "1C\r\n"
            "and this is the second one\r\n"
            "\r\n"
            "0\r\n"
            "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = 
            "GET http://127.0.0.1:9999/index.html HTTP/1.1\r\n"
            "Transfer-Encoding: chunked\r\n"
            "25\r\n"
            "This is the data in the first chunk\r\n"
            "\r\n"
            "1C\r\n"
            "and this is the second one\r\n"
            "\r\n"
            "0\r\n"
            "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = 
            "GET http://127.0.0.1:9999/index.html HTTP/1.1\r\n"
            "Transfer-Encoding: chunked\r\n"
            "2\r\n"
            "OK\r\n"
            "0\r\n"
            "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = 
            "GET http://127.0.0.1:9999/index.html HTTP/1.1\r\n"
            "Transfer-Encoding: chunked\r\n"
            "25\r\n"
            "This is the data in the first chunk78\r\n"
            "1C\r\n"
            "and this is the second one??\r\n"
            "2\r\n"
            "bo\r\n"
            "2\r\n"
            "dy\r\n"
            "0\r\n"
            "\r\n"
            "2\r\n"
            "OK\r\n"
            "0\r\n"
            "\r\n",
        .valid = true,
        .body = "",
    },
};


void testResponser()
{
    HttpParserSettings settings;

    int alltest = 0;
    int passtest = 0;

    int len = sizeof(bodys) / sizeof(bodys[0]);

    for (int i = 0; i < 3; i++)
    {
        std::cout << i << ")" << std::endl;
        alltest++;

        int begin = 0;
        HttpParser parser(&settings);
        parser.setType(HTTP_TYPE_REQUEST);
        HttpRequest *result = new HttpRequest;
        int tmp = parser.execute(bodys[i].str,
                begin,
                strlen(bodys[i].str),
                result);

        bool res = (tmp == -1) ? false : true;
        if (res)
        {
            HttpResponser responser;
            responser.response(result);
        }

        delete result;
    }

    std::cout << passtest << "/" << alltest << std::endl;
}

int main()
{
    headerMeaningInit();
    testResponser();
    return 0;
}
