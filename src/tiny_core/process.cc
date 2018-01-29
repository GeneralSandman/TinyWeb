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
#include <tiny_core/worker.h>
#include <tiny_base/api.h>
#include <tiny_base/log.h>

#include <iostream>
#include <string>
#include <sys/types.h>
#include <unistd.h>

void Process::m_fInitSignal()
{
    add_signal(SIGHUP, ProcessPool::ChildSignalHandler);
    add_signal(SIGCHLD, ProcessPool::ChildSignalHandler);
    add_signal(SIGTERM, ProcessPool::ChildSignalHandler);
    add_signal(SIGINT, ProcessPool::ChildSignalHandler);
    add_signal(SIGPIPE, ProcessPool::ChildSignalHandler);
    add_signal(SIGUSR1, ProcessPool::ChildSignalHandler);
    add_signal(SIGUSR2, ProcessPool::ChildSignalHandler);
}

void Process::m_fProcessStart()
{
    pause();
}

Process::Process(const std::string &name,
                 int number)
    : m_nName(name),
      m_nNumber(number),
      m_nPid(-1),
      m_nStarted(false),
      m_nExited(false)
{
    LOG(Debug) << "class Process constructor\n";
}

void Process::start() //invoke by processpool
{
    int res = fork();
    if (res < 0)
    {
        //handle error
    }
    else if (res == 0)
    {
        //child process
        m_fInitSignal();
        m_nPid = getpid();
        m_nStarted = true;
        m_fProcessStart();
        //start to work
    }
    else if (res > 0)
    {
        //parent process
        m_nPid = res;
        m_nStarted = true;
        // return res;
    }
}

int Process::join()
{
    //FIXME:
}

Process::~Process()
{
    LOG(Debug) << "class Process destructor\n";
}