/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.dissigil.cn
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
#include <tiny_core/timerid.h>
#include <tiny_base/buffer.h>
#include <tiny_base/signalmanager.h>
#include <tiny_base/log.h>

#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <map>
#include <string>
#include <memory>
#include <boost/bind.hpp>

void test_parent_MessageCallback(Connection *con, Buffer *buf, Time time)
{

    std::cout << "[parent] get message:"
              << buf->getAll() << std::endl;
}

void period_print_test(void)
{
    std::cout << "[parent] print every second\n";
}

ProcessPool::ProcessPool()
    : m_pEventLoop(new EventLoop()),
      m_pMaster(new Master(this, m_pEventLoop.get(), 0, "master")),
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
            //Child process:
            //1.establish channel with Parent;
            //2.set signal handlers
            //3.create listen server
            m_pProcess = std::make_shared<Process>(std::to_string(i),
                                                   i, socketpairFds);
            m_pProcess->setAsChild(int(getpid()));
            m_pProcess->setSignalHandlers();
            m_pProcess->createListenServer(m_nListenSocketFd);
            goto WAIT;
        }
        else
        {
            //Parent process:
            //setting information after forking.
            //1.push socketpair
            //2.push pid
            std::cout << "create process(" << pid << ")\n";
            pair_tmp.push_back({socketpairFds[0],
                                socketpairFds[1]});
            pids_tmp.push_back(pid);
        }
    }

    //Parent process second-step:build pipe with every child process
    for (auto t : pids_tmp)
        m_nPids.push_back(t);
    for (int i=0; i<pair_tmp.size(); i++)
    {
        int pair[2];
        pair[0] = pair_tmp[i].d1;
        pair[1] = pair_tmp[i].d2;
        std::cout << "[parent]:establish connection with child(" << m_nPids[i] <<")\n";
        // SocketPair *pipe = new SocketPair(m_pEventLoop, i);
        std::shared_ptr<SocketPair> pipe(new SocketPair(m_pEventLoop.get(), pair));
        m_nPipes.push_back(pipe);
        pipe->setParentSocket(int(m_nPids[i]));
        pipe->setMessageCallback(boost::bind(&test_parent_MessageCallback, _1, _2, _3)); //FIXME:
        setSignalHandlers();
    }
    assert(m_nPids.size() == m_nPipes.size());

WAIT:
    start();
}

void ProcessPool::setSignalHandlers()
{
    std::vector<Signal> signals = {
        Signal(SIGINT, "SIGINT", "killAll", parentSignalHandler),
        Signal(SIGTERM, "SIGTERM", "killSoftly", parentSignalHandler),
        Signal(SIGCHLD, "SIGCHLD", "childdead", parentSignalHandler),
        Signal(SIGQUIT, "QIGQUIT", "quit softly", parentSignalHandler),
        Signal(SIGPIPE, "SIGPIPE", "socket close", parentSignalHandler),
        Signal(SIGHUP, "SIGHUP", "reconfigure", parentSignalHandler)};

    for (auto t : signals)
        m_nSignalManager.addSignal(t);
}

void ProcessPool::start()
{
    if (!m_pProcess)
    {
        //Parent process
        assert(m_nPipes.size() == m_nPids.size());

        for (int index = 0; index < m_nPipes.size(); index++)
        {
            std::cout << "[parent] I will send to child(" << m_nPids[index]
                      << ") message every one seconds\n";

            TimerId id1 = m_pEventLoop->runEvery(1, boost::bind(&SocketPair::writeToChild,
                                                                m_nPipes[index],
                                                                "parent send message to child"));
            // TimerId id2 = m_pEventLoop->runEvery(1, boost::bind(&period_print_test));
        }
        m_pMaster->work();
    }
    else
    {
        //Child process
        m_pProcess->start();
    }
}

void ProcessPool::killAll()
{
    //This function will be invoked by master.
    std::cout << "[parent] kill all chilern\n";
    for (auto t : m_nPids)
    {
        std::cout << "[parent] kill child(" << t << ")\n";
        int res = kill(t, SIGTERM);
        if (res == 0)
        {
            std::cout << "[parent]:kill child (" << t << ") successfully\n";
            m_fDestoryProcess(t);
        }
    }
}

void ProcessPool::killSoftly()
{
    std::cout << "[parent] kill chilern softly\n";
    //the difference with killAll
    //FIXME:
}

ProcessPool::~ProcessPool()
{

    LOG(Debug) << "class ProcessPoll destructor\n";
}
