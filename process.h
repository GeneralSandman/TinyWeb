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

#include "api.h"

#include <iostream>
#include <string>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>

typedef boost::function<void()> Fun;

enum ProcStatus
{
  Status_Started,
  Status_Exited
};

class Process : boost::noncopyable
{
private:
  std::string m_nName; //parent process is "main"
  int m_nNumber;
  pid_t m_nPid;

  bool m_nStarted;
  bool m_nExited;

  void m_fProcessStart();

  static void m_fSetupSigHandler(void)
  {
    add_signal(SIGHUP, m_fSignalHandler);
    add_signal(SIGCHLD, m_fSignalHandler);
    add_signal(SIGTERM, m_fSignalHandler);
    add_signal(SIGINT, m_fSignalHandler);
    add_signal(SIGPIPE, m_fSignalHandler);
  }

  static void m_fSignalHandler(int sig)
  {
    std::cout << sig << std::endl;
    switch (sig)
    {
    case SIGTERM:
    case SIGINT:
    {
      // m_nExited = true;
      exit(0);
      break;
    }
    default:
    {
      break;
    }
    }
  }

public:
  explicit Process(const std::string &name, int number);
  int getNumber() { return m_nNumber; }
  void setNumber(int n) { m_nNumber = n; }
  pid_t getPid()
  {
    if (m_nPid == 0)
      m_nPid = getpid();
    return m_nPid;
  }
  int join();
  bool started() { return true == m_nStarted; }
  const std::string &getName() { return m_nName; }

  ~Process();
};

#endif // !PROCESS_H
