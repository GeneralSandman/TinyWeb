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

#include <tiny_core/process.h>
#include <tiny_core/processpool.h>
#include <tiny_core/slave.h>
#include <tiny_core/eventloop.h>
#include <tiny_core/timerid.h>
#include <tiny_base/api.h>
#include <tiny_base/log.h>

#include <iostream>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <boost/bind.hpp>

// void Process::childSignalHandler(int);
#include <tiny_base/buffer.h>
void test_child_MessageCallback(Connection *con, Buffer *buf, Time time)
{
    pid_t pid = getpid();
    LOG(Debug) << "[child] (" << pid << ") get message:"
               << buf->getAll() << std::endl;
}

void test_child_CloseCallback(Connection *con)
{
    pid_t pid = getpid();
    LOG(Debug) << "[child] (" << pid << ") connection with parent close"
               << std::endl;
}

void test_child_period_print(void)
{
    LOG(Debug) << "[child] print every second\n";
}

Process *Process::m_pProcessInstance = nullptr;

Process::Process(const std::string &name,
                 int number,
                 int sockfd[2])
    : m_pEventLoop(new EventLoop()),
      m_pSlave(new Slave(m_pEventLoop, number, name)),
      m_nName(name),
      m_nNumber(number),
      m_nPid(getpid()),
      m_nPipe(SocketPair(m_pEventLoop, sockfd))
{
    m_pProcessInstance = this;
    LOG(Debug) << "class Process constructor\n";
}

void Process::setAsChild(int port)
{
    m_nPipe.setChildSocket(port);
    m_nPipe.setMessageCallback(boost::bind(&test_child_MessageCallback, _1, _2, _3));
    m_nPipe.setCloseCallback(boost::bind(&test_child_CloseCallback, _1));
}

void Process::createListenServer(int listen)
{
    m_pSlave->createListenServer(listen);
}

void Process::setSignalHandlers()
{
    std::vector<Signal> signals = {
        Signal(SIGINT, "SIGINT", "killAll", childSignalHandler),
        Signal(SIGTERM, "SIGTERM", "killSoftly", childSignalHandler),
        Signal(SIGUSR1, "SIGUSR1", "restart", childSignalHandler),
        Signal(SIGUSR2, "SIGUSR2", "reload", childSignalHandler),
        Signal(SIGQUIT, "QIGQUIT", "quit softly", childSignalHandler),
        Signal(SIGPIPE, "SIGPIPE", "socket close", childSignalHandler),
        Signal(SIGHUP, "SIGHUP", "reconfigure", childSignalHandler),
    };

    for (auto t : signals)
        m_nSignalManager.addSignal(t);
}

void Process::start()
{
    m_pEventLoop->runEvery(1, boost::bind(&test_child_period_print));

    status = 1;

    while (status)
    {
        m_pSlave->work();

        if (status_terminate || status_quit_softly || status_restart || status_reconfigure)
            status = 0;
    }
}

pid_t Process::getPid()
{
    return m_nPid;
}

bool Process::started()
{
    // TODO:
    return true;
}

int Process::join()
{
    // TODO:
    return 0;
}

Process::~Process()
{
    m_nPipe.clearSocket();
    delete m_pSlave;
    delete m_pEventLoop;
    LOG(Debug) << "class Process destructor\n";
}
