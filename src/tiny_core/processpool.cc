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

#include <tiny_core/processpoll.h>
#include <tiny_core/process.h>
#include <tiny_core/worker.h>
#include <tiny_base/log.h>

#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <map>

// std::map<Process *, ProcStatus> ProcessPool::m_nProcess;
// int ProcessPool::m_nProNums = 0;
// bool ProcessPool::m_nStarted = false;
// bool ProcessPool::m_nStoped = false;

void ProcessPool::m_fInitSignal()
{
    add_signal(SIGHUP, ProcessPool::ParentSignalHandler);
    add_signal(SIGCHLD, ProcessPool::ParentSignalHandler);
    add_signal(SIGTERM, ProcessPool::ParentSignalHandler);
    add_signal(SIGINT, ProcessPool::ParentSignalHandler);
    add_signal(SIGPIPE, ProcessPool::ParentSignalHandler);
    add_signal(SIGUSR1, ProcessPool::ParentSignalHandler);
    add_signal(SIGUSR2, ProcessPool::ParentSignalHandler);
}

ProcessPool::ProcessPool()
// : m_pMaster(master)
{
    // m_fSetupSigHandler();
    m_fInitSignal();
    LOG(Debug) << "class ProcessPoll constructor\n";
}

void ProcessPool::start(int nums)
{
    std::cout << "master pid:" << getpid() << std::endl;
    for (int i = 0; i < nums; i++)
    {
        Process *newProc = new Process(std::to_string(i), i);
        newProc->start();
        m_nProcess[newProc] = newProc->getPid();
        std::cout << "worker pid:" << newProc->getPid() << std::endl;
    }
}

void ProcessPool::killAll()
{
    for (auto t : m_nProcess)
    {
        std::cout << "kill:" << t.second << std::endl;
        kill(t.second, 2);
        t.first->m_nStarted = false;
        t.first->m_nExited = true;
        delete t.first;
        // m_nProcess.erase(t);
    }
    for (auto t = m_nProcess.begin(); t != m_nProcess.end();)
    {
        m_nProcess.erase(t);
    }
}

ProcessPool::~ProcessPool()
{
    for (auto t : m_nProcess)
    {
        delete t.first;
    }
    LOG(Debug) << "class ProcessPoll destructor\n";
}