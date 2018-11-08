/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.dissigil.cn
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
#include <memory>

#include <tiny_core/status.h>
extern int status_quit_softly; //QUIT
extern int status_terminate;   //TERM,INT
extern int status_exiting;
extern int status_reconfigure; //HUP,reboot
extern int status_child_quit;  //CHLD

class Master;
class SocketPair;
class EventLoop;

struct pair
{
  int d1;
  int d2;
};

#include <tiny_base/buffer.h>
void test_parent_MessageCallback(Connection *con, Buffer *buf, Time time);

void period_print_test(void);

class ProcessPool
{

private:
  // EventLoop *m_pEventLoop;
  // Master *m_pMaster;
  // Process *m_pProcess;

  std::shared_ptr<EventLoop> m_pEventLoop;
  std::shared_ptr<Master> m_pMaster;
  std::shared_ptr<Process> m_pProcess;

  std::vector<std::shared_ptr<SocketPair>> m_nPipes;
  // std::vector<SocketPair *> m_nPipes;
  std::vector<pid_t> m_nPids;
  int m_nProcessNum;

  int m_nListenSocketFd;
  SignalManager m_nSignalManager;

  static void parentSignalHandler(int sign)
  {
    std::cout << "[parent]:signal manager get signal:" << sign << std::endl;
    switch (sign)
    {
    case SIGINT:
    case SIGTERM:
      status_terminate = 1;
      std::cout << "[parent]:parent will terminate all slave process\n";
      break;
    case SIGQUIT:
      status_quit_softly = 1;
      std::cout << "[parent]:quit softly\n";
    case SIGCHLD:
      status_child_quit = 1;
      int status;
      pid_t pid = waitpid(-1, &status, WNOHANG);
      std::cout << "[parent]:collect information from child[" << pid << "]\n";
      break;
      //invoke waitpid() to collect the resource of child
      // case SIGHUP:
      //   status_reconfigure = 1;
      //   std::cout << "[parent]:reconfigure\n";
      //   //kill childern softly and create new process
      //   break;
      // case SIGPIPE:; //i
      //   norncppe      break;
    }
  }

  void m_fDestoryProcess(pid_t pid)
  {
    int index = 0;
    for (; index < m_nPids.size(); index++)
    {
      if (m_nPids[index] = pid)
        break;
    }
    assert(index!=m_nPids.size());

    m_nPipes[index]->clearSocket();
    m_nPipes.erase(m_nPipes.begin() + index);
    m_nPids.erase(m_nPids.begin() + index);
    m_nProcessNum--;
  }

public:
  ProcessPool();
  void init();
  void createProcess(int nums);
  inline int processNum() { return m_nProcessNum; }
  void setSignalHandlers();
  void start();
  void killAll();
  void killSoftly();
  ~ProcessPool();
  friend class Process;
};

#endif
