#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <string>
#include <vector>
#include "api.h"

namespace server
{
class Server
{
  private:
    int server_socket;
    std::vector<int> client_sockets;

  public:
    Server(const std::string &host, const int &port, const int &listen = 8);
    int getSockfd(void);
    int getClient(void);
    ~Server() { Close(server_socket); }
};

class Factory
{
};

class Protocal
{
  private:
    int client_socket;
    int server_socket;

    std::string host;
    int port;

  public:
    int makeConnection(void);
    int loseConnection(void);

    int connectionMade(void);
    int connectionLose(void);

    int writeString(const std::string &);
};

class Service
{
};
}

#endif