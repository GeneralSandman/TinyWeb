#include "process.h"
#include "log.h"
#include <iostream>
#include <string>
#include <sys/types.h>
#include <unistd.h>

Process::Process(const std::string &name, int number)
{
    int res = fork();
    if (res > 0)
    {
        return;
    }
    else if (res == 0)
    {
        m_nName = name;
        m_nNumber = number;
        m_nPid = getpid();
        m_nStarted = false;
        m_nExited = false;
        m_fSetupSigHandler();
        LOG(Debug) << "class Process constructor\n";
        m_fProcessStart();
    }
}
void Process::m_fProcessStart()
{
    if (-1 != m_nNumber)
    {
        //child
        m_nStarted = true;
        std::cout << "child process:" << m_nName << std::endl;
        pause();
        exit(0);
    }
    else
    {
    }
}
int Process::join() {}

Process::~Process()
{
    LOG(Debug) << "class Process destructor\n";
}