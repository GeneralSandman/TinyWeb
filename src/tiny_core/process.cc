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

#include <tiny_core/process.h>
#include <tiny_core/processpool.h>
#include <tiny_core/slave.h>
#include <tiny_core/eventloop.h>
#include <tiny_base/api.h>
#include <tiny_base/log.h>

#include <iostream>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <boost/bind.hpp>

// void Process::childSignalHandler(int);


Process::Process(const std::string &name,
                 int number,
                 int sockfd[2])
    : m_pEventLoop(new EventLoop()),
      m_pSlave(new Slave(m_pEventLoop, number, name)),
      m_nName(name),
      m_nNumber(number),
      m_nPid(getpid()),
      m_nPipe(SocketPair(m_pEventLoop, sockfd)),
      m_nStarted(false),
      m_nExited(false)
{
  LOG(Debug) << "class Process constructor\n";
}

void Process::setAsChild()
{
  m_nPipe.setChildSocket();
  m_nPipe.setMessageCallback(boost::bind(&test__MessageCallback, _1, _2, _3));
}

void Process::createListenServer(int listen)
{
  m_pSlave->createListenServer(listen);
}

void Process::setSignalHandlers()
{
  std::vector<Signal> signals = {
      Signal(SIGINT, "SIGINT", "killAll", childSignalHandler),
      Signal(SIGTERM, "SIGTERM", "killSoftly", childSignalHandler)};

  for (auto t : signals)
    m_nSignalManager.addSignal(t);
}

void Process::start()
{
  m_pSlave->work();
}

pid_t Process::getPid()
{
  return m_nPid;
}

bool Process::started()
{
  return true == m_nStarted;
}

int Process::join()
{
}

Process::~Process()
{
  m_nPipe.clearSocket();
  delete m_pEventLoop;
  LOG(Debug) << "class Process destructor\n";
}