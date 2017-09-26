#ifndef SOCKET_H
#define SOCKET_H

class NetAddress;

class Socket
{
private:
  int m_nFd;

public:
  Socket(int);
  int getFd() { return m_nFd; }
  void bindAddress(const NetAddress &);
  void listen();
  int accept(NetAddress &res);
  ~Socket();
};

#endif