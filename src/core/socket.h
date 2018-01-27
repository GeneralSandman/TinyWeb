/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*---XXX---
*
****************************************
*
*/

#ifndef SOCKET_H
#define SOCKET_H

class NetAddress;

class Socket
{
private:
  int m_nFd;
  bool m_nClosed;

public:
  Socket(int);
  int getFd() { return m_nFd; }
  void bindAddress(const NetAddress &);
  void listen();
  int accept(NetAddress &per);
  int connect(const NetAddress &per);
  void shutdownWrite();
  void close();
  void setNoDelay();
  void setDelay();
  void setKeepAlive();
  void setNoKeepAlive();
  ~Socket();
};

#endif