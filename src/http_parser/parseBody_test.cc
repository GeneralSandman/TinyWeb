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

using namespace std;

vector<string> bodys = {
    "<HTML><HEAD><meta http-equiv=\"content-type\" content=\"text/html;charset=utf-8\">\n"
    "<TITLE>301 Moved</TITLE></HEAD><BODY>\n"
    "<H1>301 Moved</H1>\n"
    "The document has moved\n"
    "<A HREF=\"http://www.google.com/\">here</A>.\r\n"
    "</BODY></HTML>\r\n",

    "25  \r\n"
    "This is the data in the first chunk\r\n"
    "\r\n"
    "1C\r\n"
    "and this is the second one\r\n"
    "\r\n"
    "0  \r\n"
    "\r\n",

    "2\r\n"
    "OK\r\n"
    "0\r\n"
    "\r\n",

    "25  \r\n"
    "This is the data in the first chunk78\r\n"
    "1C\r\n"
    "and this is the second one??\r\n"
    "2\r\n"
    "bo\r\n"
    "2\r\n"
    "dy\r\n"
    "0\r\n"
    "\r\n"
    "proto"};

void testPraseBody()
{
    HttpParserSettings settings;

    int alltest = 0;
    int passtest = 0;

    int len = sizeof(bodys) / sizeof(bodys[0]);

    HttpParser parser(&settings);
    parser.setType(HTTP_REQUEST);

    for (int i = 3; i < 4; i++)
    {
        alltest++;
        int begin = 0;
        int tmp = parser.parseBody(bodys[i].c_str(),
                                   begin,
                                   bodys[i].size(),
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
