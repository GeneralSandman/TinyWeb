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

#ifndef PROCESS_H
#define PROCESS_H

#include <tiny_base/signalmanager.h>
#include <tiny_base/api.h>
#include <tiny_core/socketpair.h>

#include <iostream>
#include <string>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>

#include<tiny_core/status.h>
extern int status_quit_softly; //QUIT
extern int status_terminate;   //TERM,INT
extern int status_exiting;
extern int status_reconfigure; //HUP,reboot
extern int status_child_quit;  //CHLD

typedef boost::function<void()> Fun;

#include <tiny_base/buffer.h>
void test_child_MessageCallback(Connection *con, Buffer *buf, Time time);


enum ProcStatus
{
  Status_Started,
  Status_Exited
};

class EventLoop;
class Slave;


class Process : boost::noncopyable
{
private:
  EventLoop *m_pEventLoop;
  Slave *m_pSlave;
  std::string m_nName;
  int m_nNumber;
  pid_t m_nPid;
  SocketPair m_nPipe;
  bool m_nStarted;
  bool m_nExited;

  SignalManager m_nSignalManager;

  static void childSignalHandler(int sign)
  {
    std::cout << "child[" << getpid() << "]signal manager get signal:" << sign << std::endl;

    switch (sign)
    {
    case SIGINT:
      status_terminate = 1;
      break;
    case SIGTERM:
      status_terminate = 1;
      break;
    }
  }

public:
  Process(const std::string &name,
          int number,
          int sockfd[2]);
  void setAsChild();
  void createListenServer(int listen);
  void setSignalHandlers();
  void start();
  pid_t getPid();
  bool started();
  int join();
  ~Process();
  friend class ProcessPool;
};

#endif // !PROCESS_H
