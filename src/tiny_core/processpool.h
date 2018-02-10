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

#ifndef PROCESS_POOL_H
#define PROCESS_POOL_H

#include <tiny_core/process.h>
#include <tiny_base/signalmanager.h>
#include <tiny_base/api.h>

#include <vector>
#include <map>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>



int status_quit_softly; //QUIT
int status_terminate;   //TERM,INT
int status_exiting;
int status_reconfigure; //HUP,reboot
int status_child_quit;  //CHLD

class Master;
class SocketPair;
class EventLoop;

struct pair
{
  int d1;
  int d2;
};

#include <tiny_base/buffer.h>
void test__MessageCallback(Connection *con, Buffer *buf, Time time)
{
  std::cout << buf->getAll() << std::endl;
}

class ProcessPool
{

private:
  EventLoop *m_pEventLoop;
  Master *m_pMaster;
  Process *m_pProcess;
  std::vector<SocketPair *> m_nPipes;
  int m_nListenSocketFd;
  SignalManager m_nSignalManager;

  static void parentSignalHandler(int sign)
  {
    std::cout << "parent signal manager get signal:" << sign << std::endl;
    switch (sign)
    {
    case SIGINT:
      status_terminate = 1;
      break;
    case SIGTERM:
      status_terminate = 1;
      break;
    case SIGCHLD:
      status_child_quit = 1;
      break;
    }
  }

public:
  ProcessPool();
  void init();
  void createProcess(int nums);
  void setSignalHandlers();
  void start();
  void killAll();
  void killSoftly();
  ~ProcessPool();
  friend class Process;
};

#endif
