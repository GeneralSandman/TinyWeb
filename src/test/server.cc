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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fcntl.h>
#include <string>
#include <vector>
#include <map>
#include <sys/epoll.h>
#include <signal.h>
#include <iostream>

using namespace std;

// int main__(int argc, char **argv)
// {
//     std::map<char, std::string> opt;
//     opt = getOption(argc, argv);
//     if (opt.size() == 0)
//     {
//         std::cout << "argv error\n";
//         exit(-1);
//     }

//     std::string host = "127.0.0.1";
//     int port;
//     port = stoi(opt['p']);
//     server::Server s = server::Server(host, port);
//     std::string html = "home.html";
//     while (1)
//     {
//         server::Protocal port = s.getClient();
//         port.responseHtml(html);
//     }
// }

static void signal_handler(int sig)
{
    std::cout << "receive signal:" << sig << std::endl;
}

int main(int argc, char **argv)
{
    std::map<char, std::string> opt;
    int port = 9999;

    struct sockaddr_in serveraddr, clientaddr;
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    char *local_addr = "127.0.0.1";
    inet_aton(local_addr, &(serveraddr.sin_addr));
    serveraddr.sin_port = htons(port); //或者htons(SERV_PORT);
    int res = bind(listenfd, (const struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (res == -1)
    {
        cout << "bind error\n";
        exit(-1);
    }

    listen(listenfd, 8);
    for (;;)
    {
        socklen_t clilen;
        int connfd = accept(listenfd, (sockaddr *)&clientaddr, &clilen);
        if (connfd > 0)
        {
            cout << "get a connection\n"
                 << endl;
        }
        char *str = inet_ntoa(clientaddr.sin_addr);
        char line[8192];
        int n;
        if ((n = read(connfd, line, 8192)) < 0)
            close(connfd);
        cout << line << endl;
        char buf[8192];
        sprintf(buf, "HTTP/1.0 200 OK\r\n"); //line:netp:servestatic:beginserve
        sprintf(buf, "%sContent-Type: text/html\r\n\r\n", buf);
        write(connfd, buf, strlen(buf));
    }
}
