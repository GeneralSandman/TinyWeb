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

using namespace std;

int main()
{
    string body = "<HTML><HEAD><meta http-equiv=\"content-type\" content=\"text/html;charset=utf-8\">\n"
                  "<TITLE>301 Moved</TITLE></HEAD><BODY>\n"
                  "<H1>301 Moved</H1>\n"
                  "The document has moved\n"
                  "<A HREF=\"http://www.google.com/\">here</A>.\r\n"
                  "</BODY></HTML>\r\n";

    cout << body.size() << std::endl; //219

    string s = "0123456789";
    string s2 = "ABCDEFG";
    string s3 = "abcdefg";

    for (auto t : s)
        cout << getHex(t) << endl;
    for (auto t : s2)
        cout << getHex(t) << endl;
    for (auto t : s3)
        cout << getHex(t) << endl;
}