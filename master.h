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
*
*/
#ifndef MASTER_H
#define MASTER_H

#include "netaddress.h"

#include <string>

class EventLoop;
class Configer;
class Server;
class Protocol;

class Master
{
private:
  static EventLoop *m_pEventLoop;

  std::string m_nConfigFile;
  Configer *m_pConfiger;
  
  NetAddress m_nAddress;
  Protocol *m_pProtocol;
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
