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

#include "processpool.h"
#include "process.h"
#include "worker.h"
#include "log.h"

// std::map<Process *, ProcStatus> ProcessPool::m_nProcess;
// int ProcessPool::m_nProNums = 0;
// bool ProcessPool::m_nStarted = false;
// bool ProcessPool::m_nStoped = false;

ProcessPool::ProcessPool(Master *master)
    : m_pMaster(master)
{
    // m_fSetupSigHandler();
    LOG(Debug) << "class ProcessPoll constructor\n";
}

void ProcessPool::start(int nums)
{
    for (int i = 0; i < nums; i++)
    {
        Process *newProc = new Process(std::to_string(i), i, m_pWorker);
        m_nProcess[newProc] = Status_Started;
    }

    std::cout << "parent exited\n";
}

ProcessPool::~ProcessPool()
{
    for (auto t : m_nProcess)
    {
        delete t.first;
    }
    LOG(Debug) << "class ProcessPoll destructor\n";
}