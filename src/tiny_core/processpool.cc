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

#include <tiny_core/processpool.h>
#include <tiny_core/process.h>
#include <tiny_core/eventloop.h>
#include <tiny_core/master.h>
#include <tiny_base/log.h>

#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <map>
#include <string>
#include <boost/bind.hpp>

#include <tiny_base/buffer.h>
void test_parent_MessageCallback(Connection *con, Buffer *buf, Time time)
{
    std::cout << buf->getAll() << std::endl;
}

ProcessPool::ProcessPool()
    : m_pEventLoop(new EventLoop()),
      m_pMaster(new Master(this, m_pEventLoop, 0, "master")),
      m_pProcess(nullptr),
      m_nListenSocketFd(-1)
{

    LOG(Debug) << "class ProcessPoll constructor\n";
}

void ProcessPool::init()
{
    m_pMaster->init();
    m_nListenSocketFd = m_pMaster->getListenSocket();
}

void ProcessPool::createProcess(int nums)
{
    std::vector<pair> pair_tmp;
    std::vector<pid_t> pids_tmp;
    //first-step:create nums process
    for (int i = 0; i < nums; i++)
    {
        int socketpairFds[2];
        int res = socketpair(AF_UNIX, SOCK_STREAM, 0, socketpairFds);
        if (res == -1)
            handle_error("socketpair error:");

        pid_t pid = fork();
        if (pid < 0)
        {
            std::cout << "fork error\n";
        }
        else if (pid == 0)
        {
            //child
            m_pProcess = new Process(std::to_string(i), i, socketpairFds);
            m_pProcess->setAsChild();
            m_pProcess->setSignalHandlers();
            m_pProcess->createListenServer(m_nListenSocketFd);
            goto WAIT;
        }
        else
        {
            //parent continue
            std::cout << "create process:[" << pid << "]\n";
            tmp.push_back({socketpairFds[0],
                           socketpairFds[1]});
        }
    }

    //second-step:build pipe with every child process
    m_pEventLoop = new EventLoop();
    for (auto t : pair_tmp)
    {
        int i[2];
        i[0] = t.d1;
        i[1] = t.d2;
        std::cout << "parent establish connection with child\n";
        SocketPair *pipe = new SocketPair(m_pEventLoop, i);
        m_nPipes.push_back(pipe);
        pipe->setParentSocket();
        pipe->setMessageCallback(boost::bind(&test_parent_MessageCallback, _1, _2, _3)); //FIXME:
        setSignalHandlers();
    }
    for (auto t : pids_tmp)
    {
        m_nPids.push_back(t);
    }

WAIT:
    start();
}

void ProcessPool::setSignalHandlers()
{
    std::vector<Signal> signals = {
        Signal(SIGINT, "SIGINT", "killAll", parentSignalHandler),
        Signal(SIGTERM, "SIGTERM", "killSoftly", parentSignalHandler),
        Signal(SIGCHLD, "SIGCHLD", "childdead", parentSignalHandler)};

    for (auto t : signals)
        m_nSignalManager.addSignal(t);
}

void ProcessPool::start()
{
    //parent
    std::cout << "start work\n";
    if (!m_pProcess)
        m_pMaster->work();
    else
        m_pProcess->start();
}

void ProcessPool::killAll()
{
    std::cout << "[parent]:kill all chilern\n";
    for (auto t : m_nPids)
    {
        std::cout << "[parent]:kill child[" << t << "]\n";
        int res = kill(t, SIGTERM);
        if (res == 0)
        {
            std::cout << "[parent]:kill child[" << t << "] successfully\n";
            m_fDestoryProcess(t);
        }
    }
}

void ProcessPool::killSoftly()
{
}

ProcessPool::~ProcessPool()
{
    delete m_pMaster;
    delete m_pEventLoop;

    for (auto t : m_nPipes)
    {
        t.clearSocket();
        delete t;
    }

    LOG(Debug) << "class ProcessPoll destructor\n";
}