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

#include <tiny_base/api.h>

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

class ProcessPool;
class Worker;

class Process : boost::noncopyable
{
private:
  // Worker *m_pWorker;
  std::string m_nName; //parent process is "main"
  int m_nNumber;
  pid_t m_nPid;
  bool m_nStarted;
  bool m_nExited;

  void m_fInitSignal();
  void m_fProcessStart();

public:
  explicit Process(const std::string &name, int number);
  void start(); //invoke by processpool
  pid_t getPid()
  {
    return m_nPid;
  }
  int join();
  bool started() { return true == m_nStarted; }
  ~Process();

  friend class ProcessPool;
};

#endif // !PROCESS_H
