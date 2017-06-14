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
#include "./server/server.h"
#include "./api/api.h"
void printf_connection(struct sockaddr_in &addr);
void response_header(int client_socket);
void response_html(int client_socket);

int main(int argc, char **argv)
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
    std::string html = "1";

    while (1)
    {
        std::cout<<std::endl;
        server::Protocal port = s.getClient();
        port.writeString(html);
        html+="1";
    }
}
