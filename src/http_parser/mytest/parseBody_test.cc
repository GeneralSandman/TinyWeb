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

#include "../http_parser.h"

#include <iostream>
#include <vector>
#include <string>

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
            "GET http://127.0.0.1:9999/index.html HTTP/1.1\r\n"
            "Transfer-Encoding: chunked\r\n"
            "Content-Length: 16\r\n"
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
        .valid = false,
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


void testPraseBody()
{
    HttpParserSettings settings;

    int alltest = 0;
    int passtest = 0;

    int len = sizeof(bodys) / sizeof(bodys[0]);

    HttpParser parser(&settings);
    parser.setType(HTTP_TYPE_REQUEST);

    for (int i = 0; i < len; i++)
    {
        alltest++;
        int begin = 0;
        int tmp = parser.parseBody(bodys[i].str,
                begin,
                strlen(bodys[i].str),
                true);

        bool res = (tmp == -1) ? false : true;

        if (res)
        {
            cout << "body valid\n";
        }
        else
        {
            cout << "body invalid\n";
        }
    }

    std::cout << passtest << "/" << alltest << std::endl;
}

int main()
{
    testPraseBody();
    return 0;
}
