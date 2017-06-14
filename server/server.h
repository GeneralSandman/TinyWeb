#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <string>
#include <vector>
#include "../api/api.h"

namespace server
{
class Protocal;
class Server
{
private:
  int server_socket;
  std::vector<int> client_sockets;

public:
  Server(const std::string &host, const int &port, const int &listen = 8);
  int getSockfd(void);
  Protocal getClient(void);
  ~Server();
};

class Factory
{
};

class Protocal
{

private:
  //int server_socket;
  int client_socket;

  std::string peer;
  int peer_port;

  void response_header(int client_socket);
  void response_html(int client_socket);

public:
  Protocal(const int &, const std::string &, const int &);
  ~Protocal();
  int makeConnection(void);
  int loseConnection(void);

  int connectionMade(void);
  int connectionLose(void);

  size_t writeString(const std::string &);
  size_t writeHtml(const std::string &);
};

class Service
{
};
}

#endif