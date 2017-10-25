/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*---class Master---
*
* this class will do those things:
* switch to daemon process
* new Protocol,new Server;
* server->start() and eventloop->loop();
****************************************
*Server():initalize the Server and Protocol
*start():invoke Server::start(),EventLoop::loop()
*~Server():delete the resource;
*/
#ifndef MASTER_H
#define MASTER_H

#include "netaddress.h"

#include <string>

class EventLoop;
class Server;
class Factory;

void logSecond();

class Master
{
private:
  static EventLoop *m_pEventLoop;
  std::string m_nConfigFile;
  NetAddress m_nAddress;
  Factory *m_pFactory;
  Server *m_pServer;

  void m_fSwitchtoDaemon();
  void m_fInit();
  static void m_fSignalHandler(int);

public:
  Master(const std::string &configfile);
  void start();
  ~Master();
};

#endif // !MASTER_H
