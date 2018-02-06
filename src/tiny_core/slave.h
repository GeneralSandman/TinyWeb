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

#ifndef SLAVE_H
#define SLAVE_H

#include <string>

class EventLoop;
class Protocol;
class Factory;
class Server;

class Slave
{
protected:
  EventLoop *m_pEventLoop;
  int m_nNumber;
  std::string m_nName;

  Protocol *m_pProtocol;
  Factory *m_pFactory;

  NetAddress m_nListenAddress;
  int m_nListenSocketFd;
  Server *m_pServer;

public:
  Slave(EventLoop *, int, const std::string &);
  void createListenServer(int listenSocket)
  {
    m_nListenSocketFd=listenSocket;
    m_pServer = new Server(m_pEventLoop,
                           m_nListenAddress, listenSocket, m_pFactory);
  }
  void work();
  ~Slave();
};

#endif //!SLAVE_H