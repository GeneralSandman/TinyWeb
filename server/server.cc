#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "../api/server.h"
#include "../api/api.h"
void printf_connection(struct sockaddr_in &addr);
void response_header(int client_socket);
void response_html(int client_socket);



int main__(int argc, char **argv)
{
    int i;
    char buf[1024];

    int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    serv_addr.sin_port = htons(9999);

    Bind(serv_sock, (const struct sockaddr *)&serv_addr, sizeof(serv_addr));
    Listen(serv_sock, 8);

    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);

    while (1)
    {
        int client_socket = accept(serv_sock, (struct sockaddr *)&client_addr, &client_addr_size);
        printf_connection(client_addr);
        response_header(client_socket);
        response_html(client_socket);

        shutdown(client_socket, SHUT_WR);
        // close(client_socket);
    }

    close(serv_sock);

    return 0;
}

int main(int argc, char **argv)
{
    std::map<char, std::string> opt;
    opt = getOption(argc, argv);
    if(opt.size()==0){
        std::cout<<"argv error\n";
        exit(-1);
    }

    std::string host = "127.0.0.1";
    int port;
    port = stoi(opt['p']);
    server::Server s = server::Server(host, port);

    while (1)
    {
        int client_socket = s.getClient();
        response_header(client_socket);
        response_html(client_socket);
        std::cout << "----" << std::endl;
        shutdown(client_socket, SHUT_WR);
    }
}

void printf_connection(struct sockaddr_in &addr)
{
    char buff[1024];
    printf("connection from %s,port:%d\n", inet_ntop(AF_INET, &addr.sin_addr, buff, sizeof(buff)),
           ntohs(addr.sin_port));
}

void response_header(int client_socket)
{
    using namespace std;
    vector<string> header;
    header.reserve(3);
    header.push_back("HTTP/1.0 200 Method Not Implemented\r\n");
    header.push_back("Content-Type: text/html\r\n");
    header.push_back("\r\n");
    for (auto t : header)
    {
        Write(client_socket, t);
    }
}

void response_html(int client_socket)
{
    using namespace std;
    vector<string> header;
    header.reserve(4);
    header.push_back("<HTML><HEAD><TITLE>Method Not Implemented\r\n");
    header.push_back("</TITLE></HEAD>\r\n");
    header.push_back("<BODY><P>HTTP request method not supported.\r\n");
    header.push_back("</BODY></HTML>\r\n");
    cout << header.capacity();
    for (auto t : header)
    {
        Write(client_socket, t);
    }
}
