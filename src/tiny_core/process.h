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
class EventLoop;

class Process : boost::noncopyable
{
private:
  Worker* m_pWorker;
  EventLoop *m_pEventLoop;
  std::string m_nName;
  int m_nNumber;
  pid_t m_nPid;
  SocketPair m_nPipe;
  bool m_nStarted;
  bool m_nExited;

  void m_fInitSignal();
  void m_fProcessStart();

public:
  Process(const std::string &name,
          int number,
          int sockfd[2])
      : m_pEventLoop(new EventLoop),
        m_nName(name),
        m_nNumber(number),
        m_nPid(getpid()),
        m_nPipe(m_pEventLoop, sockfd),
        m_nStarted(false),
        m_nExited(false)
  {
    LOG(Debug) << "class Process constructor\n";
  }
  void setAsChild()
  {
    m_nPipe.setChildSocket();
    m_nPipe.setMessageCallback();
  }
  void start() { m_nWorker.work(); }
  pid_t getPid() { return m_nPid; }
  bool started() { return true == m_nStarted; }
  int join() {}
  ~Process()
  {
    m_nPipe.clearSocket();
    delete m_pEventLoop;
    LOG(Debug) << "class Process destructor\n";
  }
  friend class ProcessPool;
};

#endif // !PROCESS_H
