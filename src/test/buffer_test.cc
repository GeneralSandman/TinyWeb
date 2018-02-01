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

#include <tiny_base/buffer.h>

#include <iostream>

using namespace std;

int main()
{
    {
        Buffer out;
        out.append("abc");
        out.append("abc");
        out.append("abc");
        out.append("abc");
        out.append("abc");
        cout << out.get(3) << endl;
        cout << out.getAll() << endl;
    }

    {
        Buffer out;
        vector<string> request;

        std::string httpRequest = "GET / HTTP/1.0\r\n";
        std::string httpRequest2 = "Host: localhost:9595\r\n";
        std::string httpRequest3 = "Accept: text/html\r\n";
        std::string httpRequest4 = "Accept-Encoding: gzip, compress, bzip2\r\n";
        std::string httpRequest5 = "Accept-Language: en\r\n";
        std::string httpRequest6 = "User-Agent: Lynx/2.8.9dev.8 libwww-FM/2.14 SSL-MM/1.4.1 GNUTLS/3.4.9\r\n";
        std::string end = "\r\n";

        out.append(httpRequest);
        out.append(httpRequest2);
        out.append(httpRequest3);
        out.append(httpRequest4);
        out.append(httpRequest5);
        out.append(httpRequest6);
        out.append(end);

        string line;
        int i = 0;
        while (out.getALine(line))
        {
            std::cout << i++ << "-" << line << "-\n";
            line = "";
        }
    }

    return 0;
}