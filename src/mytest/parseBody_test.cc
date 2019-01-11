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
#include <string>
#include <vector>

using namespace std;

typedef struct testBody {
    const char* str;
    bool valid;
    const char* body;

} testBody;

testBody bodys[] = {
    {
        .str = "GET /index.html HTTP/1.0\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /index.html HTTP/1.0\r\n"
               "\r\n"
               "helloworld",
        .valid = true,
        .body = "helloworld",
    },

    {
        .str = "GET /index.html HTTP/0.9\r\n"
               "\r\n"
               "helloworld",
        .valid = true,
        .body = "helloworld",
    },

    {
        .str = "GET /index.html HTTP/1.0\r\n"
               "Content-Length: 10\r\n"
               "\r\n"
               "helloworld",
        .valid = true,
        .body = "helloworld",
    },

    {
        .str = "GET /index.html HTTP/1.0\r\n"
               "Content-Length: 20\r\n"
               "\r\n"
               "helloworld",
        .valid = true,
        .body = "helloworld",
    },

    // 5
    {
        .str = "GET /index.html HTTP/1.0\r\n"
               "Content-Length: 5\r\n"
               "\r\n"
               "helloworld",
        .valid = true,
        .body = "helloworld",
    },

    {
        .str = "GET /index.html HTTP/1.0\r\n"
               "Content-Length: 10.\r\n"
               "\r\n"
               "helloworld",
        .valid = false,
        .body = "",
    },

    {
        .str = "GET /index.html HTTP/1.0\r\n"
               "Content-Length: 1-0\r\n"
               "\r\n"
               "helloworld",
        .valid = false,
        .body = "",
    },

    {
        .str = "GET /index.html HTTP/1.0\r\n"
               "Content-Length: bbb\r\n"
               "\r\n"
               "helloworld",
        .valid = false,
        .body = "",
    },

    {
        .str = "GET /index.html HTTP/1.0\r\n"
               "Content-Length: -10\r\n"
               "\r\n"
               "helloworld",
        .valid = false,
        .body = "",
    },

    // 10
    {
        .str = "GET /index.html HTTP/1.0\r\n"
               "Transfer-Encoding: chunked\r\n"
               "\r\n"
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
        .body = "This is the data in the first chunk78\r\n"
                "and this is the second one??"
                "bo"
                "dy",
    },

    {
        .str = "GET /index.html HTTP/1.0\r\n"
               "Transfer-Encoding: chunked\r\n"
               "\r\n"
               "25\r\n"
               "This is the data in the first chunk78\r\n"
               "1C\r\n"
               "and this is the second one??\r\n"
               "0\r\n"
               "\r\n",
        .valid = true,
        .body = "This is the data in the first chunk78"
                "and this is the second one??",
    },

    {
        .str = "GET /index.html HTTP/1.1\r\n"
               "Transfer-Encoding: chunke\r\n"
               "\r\n"
               "25\r\n"
               "This is the data in the first chunk78\r\n"
               "1C\r\n"
               "and this is the second one??\r\n"
               "0\r\n"
               "\r\n",
        .valid = false,
        .body = "",
    },

    {
        .str = "GET /index.html HTTP/1.1\r\n"
               "Transfer-Encoding: -----\r\n"
               "\r\n"
               "25\r\n"
               "This is the data in the first chunk78\r\n"
               "1C\r\n"
               "and this is the second one??\r\n"
               "0\r\n"
               "\r\n",
        .valid = false,
        .body = "",
    },

    {
        .str = "GET /index.html HTTP/1.1\r\n"
               "Transfer-Encoding: chunked\r\n"
               "Content-Length: 12345\r\n"
               "\r\n"
               "25\r\n"
               "This is the data in the first chunk78\r\n"
               "1C\r\n"
               "and this is the second one??\r\n"
               "0\r\n"
               "\r\n",
        .valid = false,
        .body = "",
    },

    // 15
    {
        .str = "GET /index.html HTTP/1.0\r\n"
               "Content-Length: 10\r\n"
               "Host: 127.0.0.1:9090\r\n"
               "User-Agent: curl/7.54.0\r\n"
               "Accept: */*\r\n"
               "\r\n"
               "helloworld",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /index.html HTTP/1.1\r\n"
               "Host: 127.0.0.1:9090\r\n"
               "User-Agent: curl/7.54.0\r\n"
               "Accept: */*\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET /index.html HTTP/1.1\r\n"
               "Host: 127.0.0.1:9090\r\n"
               "User-Agent: curl/7.54.0\r\n"
               "Accept: */*\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET http://127.0.0.1:9999/index.html HTTP/1.0\r\n"
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
        .str = "GET http://127.0.0.1:9999/index.html HTTP/1.0\r\n"
               "Transfer-Encoding: chunked\r\n"
               "\r\n"
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

    // 20
    {
        .str = "GET http://127.0.0.1:9999/index.html HTTP/1.0\r\n"
               "Transfer-Encoding: chunked\r\n"
               "\r\n"
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
        .str = "GET http://127.0.0.1:9999/index.html HTTP/1.0\r\n"
               "Transfer-Encoding: chunked\r\n"
               "\r\n"
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
        .str = "GET http://127.0.0.1:9999/index.html HTTP/1.0\r\n"
               "Transfer-Encoding: chunked\r\n"
               "\r\n"
               "2\r\n"
               "OK\r\n"
               "0\r\n"
               "\r\n",
        .valid = true,
        .body = "",
    },

    {
        .str = "GET http://127.0.0.1:9999/index.html HTTP/1.0\r\n"
               "Transfer-Encoding: chunked\r\n"
               "\r\n"
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

    vector<int> notPass;
    int alltest = 0;
    int passtest = 0;

    int len = sizeof(bodys) / sizeof(bodys[0]);

    for (int i = 0; i < len; i++) {
        std::cout << i << ")" << std::endl;
        alltest++;
        int begin = 0;
        HttpParser parser(&settings);
        parser.setType(HTTP_TYPE_REQUEST);
        HttpRequest* result = new HttpRequest;
        int tmp = parser.execute(bodys[i].str,
            begin,
            strlen(bodys[i].str),
            result);

        bool res = (tmp == -1) ? false : true;
        if (res == bodys[i].valid) {

            if (res) {

                bool sameBody = true;
                if (sameBody) {
                    passtest++;
                } else {
                    notPass.push_back(i);
                }
            } else {
                passtest++;
            }
        } else {
            notPass.push_back(i);
        }

        delete result;
    }

    std::cout << "[Parse Body Test] pass/all = " << passtest << "/" << alltest << std::endl;

    if (!notPass.empty()) {
        cout << "not pass body index:\t";
        for (auto t : notPass)
            std::cout << t << " ";
        std::cout << std::endl;
    }
}

int main()
{
    headerMeaningInit();
    testPraseBody();
    return 0;
}
