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

#include <tiny_base/log.h> 
#include <tiny_http/http_parser.h>

#include <iostream>
#include <vector>

using namespace std;

typedef struct testResponseLine {
    const char* str;
    const bool valid;
    const bool isResponse;
    const int httpVersion;
    const unsigned int statusCode;
    const char* statusPhrase;
} testResponseLine;

bool sameUrl(const Url* url, const char* str)
{
    return (0 == strncmp(url->data, str, url->len));
}

testResponseLine responseLines[] = {
    {
        .str = "HTTP/1.1 200 OK\r\n",
        .valid = true,
        .isResponse = true,
        .httpVersion = 11,
        .statusCode = 200,
        .statusPhrase = "OK",
    },

    {
        .str = "HTTP/1.0 304 Not Modified\r\n",
        .valid = true,
        .isResponse = true,
        .httpVersion = 10,
        .statusCode = 304,
        .statusPhrase = "Not Modified",
    },

    {
        .str = "HTTP/1.1 100 Continue\r\n",
        .valid = true,
        .isResponse = true,
        .httpVersion = 11,
        .statusCode = 100,
        .statusPhrase = "Continue",
    },

    {
        .str = "HTTP/1.1 201 Created\r\n",
        .valid = true,
        .isResponse = true,
        .httpVersion = 11,
        .statusCode = 201,
        .statusPhrase = "Created",
    },

    {
        .str = "HTTP/1.0 202 Accepted\r\n",
        .valid = true,
        .isResponse = true,
        .httpVersion = 10,
        .statusCode = 202,
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

    for (int i = 0; i < len; i++) {
        // std::cout << i << ")" << std::endl;
        alltest++;

        HttpParser parser(&settings);
        parser.setType(HTTP_TYPE_RESPONSE);

        int begin = 0;
        int len = strlen(responseLines[i].str);
        HttpRequest* request = new HttpRequest;

        int tmp = parser.execute(responseLines[i].str, begin, len, request);

        bool res = (tmp == -1) ? false : true;
        if (res == responseLines[i].valid) {

            if (res) {

                int version = request->http_version_major * 10 + request->http_version_minor;
                bool sameversion = (version == responseLines[i].httpVersion);

                unsigned int code = request->statusCode;
                bool sameCode = (code == responseLines[i].statusCode);

                int phraselen = strlen(responseLines[i].statusPhrase);
                bool samePhrase = (0 == strncmp(responseLines[i].statusPhrase, request->statusPhrase.c_str(), phraselen));

                if (sameversion && sameCode ) {
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

        delete request;
    }

    std::cout << "[Parse ResponseLine Test] pass/all = " << passtest << "/" << alltest << std::endl;

    if (!notPass.empty()) {
        cout << "not pass response-line index:\t";
        for (auto t : notPass)
            std::cout << t << " ";
        std::cout << std::endl;
    }
};

int main()
{
    testParseResponseLine();
    return 0;
}
