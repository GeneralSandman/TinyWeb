#ifndef SOCKET_H
#define SOCKET_H

class NetAddress;

class Socket
{
private:
  int m_nFd;

public:
  Socket(int);
  void bindAddress(NetAddress &);
  void listen();
  int accept(NetAddress &res);
  ~Socket();
};

#endif