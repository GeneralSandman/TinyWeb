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

void ProcessPool::m_fInitSignal()
{
}

ProcessPool::ProcessPool()
{
    m_fInitSignal();
    LOG(Debug) << "class ProcessPoll constructor\n";
}

void ProcessPool::createProcess(int nums)
{
    std::vector<pair> tmp;
    //first-step:create nums process
    for (int i = 0; i < nums; i++)
    {
        int sockpairFds[2];
        int res = socketpair(AF_UNIX, SOCK_STREAM, 0, sockpairFds);
        if (res == -1)
            handle_error("socketpair error:");

        int res = fork();
        if (res < 0)
        {
            std::cout << "fork error\n";
        }
        else if (res == 0)
        {
            //child
            m_pProcess = new Process(to_string(i), i, socketpairFds);
            m_pProcess->setAsChild();
            goto WAIT;
        }
        else
        {
            //parent continue
            tmp.push_back({sockpairFds[0],
                           sockpairFds[1]});
        }
    }

    //second-step:build pipe with every child process
    m_pEventLoop = new EventLoop();
    for (auto t : tmp)
    {
        int i[2];
        i[0] = t.d1;
        i[1] = t.d2;

        SocketPair *pipe = new SocketPair(m_pEventLoop, i);
        m_nPipes.push_back(pipe);
    }

WAIT:
    start();
}


void ProcessPool::killAll()
{
}

ProcessPool::~ProcessPool()
{
    LOG(Debug) << "class ProcessPoll destructor\n";
}