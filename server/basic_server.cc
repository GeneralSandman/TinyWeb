#include "server.h"
#include "../api/api.h"
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <algorithm>

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

Protocal Server::getClient(void)
{
    char buff[1024];
    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    int client_socket = accept(server_socket, (struct sockaddr *)&client_addr,
                               &client_addr_size);
    client_sockets.push_back(client_socket);
    std::string host = Inet_ntop(AF_INET, &client_addr.sin_addr, buff, sizeof(buff));
    int port = ntohs(client_addr.sin_port);
    Protocal prot = Protocal(client_socket, host, port);
    return prot;
}

Server::~Server()
{
    if (Close(server_socket) > 0)
        std::cout << "close server \n";
}



void Protocal::read_request()
{
    char buf[1024];
    char *head=buf;
    size_t numchars = get_line(client_socket, buf, sizeof(buf));
    while(*head!='\0'){
        request+=*(head++);
    }
}

void Protocal::parser_request(void){
    std::cout<<request<<std::endl;
    std::vector<std::string> res;
    splitString(request," ",res);
    // method url http  
    for(int i=0;i<res.size();i++)
        std::cout<<i<<"-"<<res[i]<<"-"<<std::endl;


}

void Protocal::response_header()
{
    using namespace std;
    vector<string> header;
    header.reserve(3);
    header.push_back("HTTP/1.0 200 OK\r\n");
    header.push_back("Content-Type: text/html\r\n\r\n");
    for (auto t : header)
    {
        writeString(client_socket, t);
    }
}

void Protocal::response_body()
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
        writeString(client_socket, t);
    }
}

Protocal::Protocal(const int &sockfd, const std::string &ip, const int &port)
{
    client_socket = sockfd;
    peer = ip;
    peer_port = port;
    std::cout << "connection:" << peer << "," << peer_port << std::endl;
    connectionMade();
}

Protocal::~Protocal()
{
    shutdown(client_socket, SHUT_WR);
    std::cout << "close connection:" << peer << ","
              << peer_port << std::endl;
}

int Protocal::makeConnection(void)
{
}

int Protocal::loseConnection(void)
{
}

int Protocal::connectionMade(void)
{
    read_request();
    parser_request();
    response_header();
}

int Protocal::connectionLose(void)
{
}

std::string stringReceive(void){

}

int stringTarnsform(const std::string &){
    
}


size_t Protocal::responseHtml(const std::string &filename)
{
    int len = writeHtml(client_socket, filename);
    return len;
}
}