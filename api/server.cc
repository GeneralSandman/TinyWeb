#include "server.h"
#include "api.h"
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>

namespace server
{
Server::Server(const std::string &host, const int &port, const int &listen)
{
    server_socket = Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = Inet_addr(host);
    serv_addr.sin_port = htons(port);

    Bind(server_socket, (const struct sockaddr *)&serv_addr, sizeof(serv_addr));
    Listen(server_socket, listen);
}

int Server::getSockfd(void)
{
    return server_socket;
}

int Server::getClient(void)
{
    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_size);
    client_sockets.push_back(client_socket);
    return client_socket;

    char buff[1024];

    std::string host = Inet_ntop(AF_INET, &client_addr.sin_addr, buff, sizeof(buff));
    int port = ntohs(client_addr.sin_port);
}
}