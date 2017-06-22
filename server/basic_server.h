#ifndef BASIC_SERVER_H
#define BASIC_SERVER_H

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

  std::string request;

  void read_request();
  void parser_request();

  void response_header();
  void response_body();

public:
  Protocal(const int &, const std::string &, const int &);
  ~Protocal();
  int makeConnection(void);
  int loseConnection(void);

  int connectionMade(void);
  int connectionLose(void);

  std::string stringReceive(void);
  int stringTransform(const std::string &);

  // size_t writeString(const std::string &);
  size_t responseHtml(const std::string &);
};

}

#endif