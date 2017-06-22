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
#include "./server/basic_server.h"
#include "./api/api.h"

int main__(int argc, char **argv)
{
    std::map<char, std::string> opt;
    opt = getOption(argc, argv);
    if (opt.size() == 0)
    {
        std::cout << "argv error\n";
        exit(-1);
    }

    std::string host = "127.0.0.1";
    int port;
    port = stoi(opt['p']);
    server::Server s = server::Server(host, port);
    std::string html = "home.html";
    while (1)
    {
        server::Protocal port = s.getClient();
        port.responseHtml(html);
    }
}

int main(int argc, char **argv)
{
    std::map<char, std::string> opt;
    opt = getOption(argc, argv);
    if (opt.size() == 0)
    {
        std::cout << "argv error\n";
        exit(-1);
    }

    int port = stoi(opt['p']);

    struct epoll_event ev, events[20];
    int epfd = epoll_create(256);

    struct sockaddr_in serveraddr, clientaddr;
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    setnonblocking(listenfd);

    ev.data.fd = listenfd;         //设置与要处理的事件相关的文件描述符
    ev.events = EPOLLIN | EPOLLET; //设置要处理的事件类型
    epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);

    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    char *local_addr = "127.0.0.1";
    inet_aton(local_addr, &(serveraddr.sin_addr));
    serveraddr.sin_port = htons(port); //或者htons(SERV_PORT);
    Bind(listenfd, (sockaddr *)&serveraddr, sizeof(serveraddr));
    Listen(listenfd, 8);
    for (;;)
    {
        int nfds = epoll_wait(epfd, events, 20, 500); //等待epoll事件的发生
        for (int i = 0; i < nfds; ++i)                //处理所发生的所有事件
        {
            if (events[i].data.fd == listenfd) //监听事件
            {
                socklen_t clilen;
                int connfd = accept(listenfd, (sockaddr *)&clientaddr, &clilen);
                if (connfd < 0)
                {
                    perror("connfd<0");
                    exit(1);
                }
                setnonblocking(connfd); //把客户端的socket设置为非阻塞方式
                char *str = inet_ntoa(clientaddr.sin_addr);
                std::cout << "connect from " << str << std::endl;
                ev.data.fd = connfd;           //设置用于读操作的文件描述符
                ev.events = EPOLLIN | EPOLLET; //设置用于注测的读操作事件
                epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);
                //注册ev事件
            }
            else if (events[i].events & EPOLLIN) //读事件
            {
                char line[8192];
                int connfd;
                if ((connfd = events[i].data.fd) < 0)
                {
                    continue;
                }
                int n;
                std::cout << "connfd is:" << connfd << std::endl;
                if ((n = read(connfd, line, 8192)) < 0) // 这里和IOCP不同
                {
                    if (errno == ECONNRESET)
                    {
                        close(connfd);
                        events[i].data.fd = -1;
                    }
                    else
                    {
                        std::cout << "readline error" << std::endl;
                    }
                }
                else if (n == 0)
                {
                    close(connfd);
                    events[i].data.fd = -1;
                }
                // printf("-%s-\n", line);

                ev.data.fd = connfd;            //设置用于写操作的文件描述符
                ev.events = EPOLLOUT | EPOLLET; //设置用于注测的写操作事件
                //修改connfd上要处理的事件为EPOLLOUT
                epoll_ctl(epfd, EPOLL_CTL_MOD, connfd, &ev);
            }
            else if (events[i].events & EPOLLOUT) //写事件
            {
                int client_socket = events[i].data.fd;
                char buf[8192];
                sprintf(buf, "HTTP/1.0 200 OK\r\n"); //line:netp:servestatic:beginserve
                sprintf(buf, "%sContent-Type: text/html\r\n\r\n", buf);
                write(client_socket, buf, strlen(buf));
                writeHtml(client_socket, "home.html");
                shutdown(client_socket, SHUT_RD);
                ev.data.fd = client_socket;    //设置用于读操作的文件描述符
                ev.events = EPOLLIN | EPOLLET; //设置用于注册的读操作事件
                //修改sockfd上要处理的事件为EPOLIN
                epoll_ctl(epfd, EPOLL_CTL_MOD, client_socket, &ev);
            }
        }
    }
}
