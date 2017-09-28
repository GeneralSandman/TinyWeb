#include "processpool.h"
#include "process.h"
#include "log.h"

std::map<Process *, ProcStatus> ProcessPool::m_nProcess;
int ProcessPool::m_nProNums = 0;
bool ProcessPool::m_nStarted = false;
bool ProcessPool::m_nStoped = false;

ProcessPool::ProcessPool()
{
    m_fSetupSigHandler();
    LOG(Debug) << "class ProcessPoll constructor\n";
}

void ProcessPool::start(int nums)
{
    for (int i = 0; i < nums; i++)
    {
        Process *newProc = new Process(std::to_string(i), m_nProNums++);
        m_nProcess[newProc] = Status_Started;
    }

    m_nStarted = true;
    pause();
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