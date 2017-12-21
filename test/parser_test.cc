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

#include "../parser.h"
#include "../api.h"

#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
using namespace std;

int main()
{
    // GET / index.htm2 HTTP / 1.0
    string method = "[A-Z_a-z]*";
    string url = "/^([\\/][\\w-]+)*$/";
    string version = "";
    string pattern = "[A-Z_a-z]*\\s\\s";

    return 0;
}

int main__()
{
    {
        HttpParser parser;

        vector<string> request;

        std::string httpRequest = "GET / HTTP/1.0";
        std::string httpRequest2 = "Host: localhost:9595";
        std::string httpRequest3 = "Accept: text/html";
        std::string httpRequest4 = "Accept-Encoding: gzip, compress, bzip2";
        std::string httpRequest5 = "Accept-Language: en";
        std::string httpRequest6 = "User-Agent: Lynx/2.8.9dev.8 libwww-FM/2.14 SSL-MM/1.4.1 GNUTLS/3.4.9";
        std::string end = "";

        request.push_back(httpRequest);
        request.push_back(httpRequest2);
        request.push_back(httpRequest3);
        request.push_back(httpRequest4);
        request.push_back(httpRequest5);
        request.push_back(httpRequest6);
        request.push_back(end);

        struct HttpRequestHeader header;
        struct HttpRequestContent content;

        for (auto t : request)
        {
            parser.parseRequestLine(t, &header, &content);
        }

        std::cout << "header values:\n"
                  << "method" << header.method << std::endl
                  << "url" << header.url << std::endl
                  << "version" << header.version << std::endl;

        std::cout << "---------\n";

        std::cout << "content values:\n"
                  << "host" << content.host << std::endl
                  << "connection" << content.connection << std::endl
                  << "user_agent" << content.user_agent << std::endl
                  << "accept" << content.accept << std::endl
                  << "cookie" << content.cookie << std::endl;
    }

    return 0;
}

int main_(int argc, char **argv)
{

    if (argc < 2)
    {
        cout << "argv error\n";
        exit(-1);
    }
    cout << "pid:" << getpid() << endl;
    const char *ip = argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    serv_addr.sin_port = htons(port);

    int listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    bind(listen_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    listen(listen_fd, 8);

    int data_recv = 0;

    struct sockaddr client_addr;
    socklen_t client_len = sizeof(client_addr);
    int connectfd = accept(listen_fd, &client_addr, &client_len);

    Parser *par = new Parser(connectfd);
    while (1)
    {
        data_recv = par->recv_data_continue();
        if (data_recv == -1)
        {
            break;
        }
        else if (data_recv == 0)
        {
            break;
        }
        else
        {
            HTTP_CODE result = par->parse_content();
            if (result == HTTP_CODE::NO_REQUEST)
            {
                //contiune recv data
                continue;
            }
            else if (result == HTTP_CODE::GET_REQUEST)
            {
                using namespace std;
                vector<string> header;
                header.reserve(3);
                header.push_back("HTTP/1.0 200 OK\r\n");
                header.push_back("Content-Type: text/html\r\n\r\n");
                for (auto t : header)
                {
                    writeString(connectfd, t);
                }
                std::string file = "../www/";
                file += par->m_nUrl;
                writeHtml(connectfd, file);

                close(connectfd);
            }
            else
            {
                //bad
            }
        }
    }

    delete par;

    return 0;
}