#include "process.h"
#include "log.h"
#include <iostream>
#include <string>
#include <sys/types.h>
#include <unistd.h>

Process::Process(const std::string &name)
{
    m_nName = name;
    m_nPid = getpid();
    m_nStarted = false;
    m_nExited = false;
    LOG(Debug) << "class Process constructor\n";
}
void Process::start()
{
    if ("main" == m_nName)
    {
        // send message
    }
    else
    {
        //wait and get message
    }
}
int Process::join() {}
bool Process::started() {}
pid_t Process::getPid() {}
const std::string &Process::getName() {}

Process::~Process()
{
    LOG(Debug) << "class Process destructor\n";
}