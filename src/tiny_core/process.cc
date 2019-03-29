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

#include <tiny_base/api.h>
#include <tiny_base/log.h>
#include <tiny_base/sync.h>
#include <tiny_core/eventloop.h>
#include <tiny_core/process.h>
#include <tiny_core/processpool.h>
#include <tiny_core/slave.h>
#include <tiny_core/timerid.h>

#include <boost/bind.hpp>
#include <iostream>
#include <string>
#include <memory>
#include <sys/types.h>
#include <unistd.h>

// void Process::childSignalHandler(int);
#include <tiny_base/buffer.h>
void test_child_MessageCallback(Connection* con, Buffer* buf, Time time)
{
    pid_t pid = getpid();
    LOG(Debug) << "[child] (" << pid << ") get message:"
               << buf->getAll() << std::endl;
}

void test_child_CloseCallback(Connection* con)
{
    pid_t pid = getpid();
    LOG(Debug) << "[child] (" << pid << ") connection with parent close"
               << std::endl;
}

void test_child_period_print(void)
{
    pid_t pid = getpid();
    LOG(Debug) << "[child] (" << pid << ") print every second\n";
}

void test_child_period_read_sharememory(Process* process)
{
    pid_t pid = getpid();
    std::string data = process->readFromSharedMemory();
    LOG(Debug) << "[child] (" << pid << ") read share-memory every second,data-size(" << data.size() << 
        "),data(" << data << ")\n";
}

Process* Process::m_pProcessInstance = nullptr;

Process::Process(const std::string& name,
    int number,
    int sockfd[2],
    std::shared_ptr<Sync> sync)
    : m_pEventLoop(new EventLoop())
    , m_pSlave(new Slave(m_pEventLoop, number, name))
    , m_nName(name)
    , m_nNumber(number)
    , m_nPid(getpid())
    , m_nPipe(SocketPair(m_pEventLoop, sockfd))
    , m_pSync(sync)
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

void Process::createListenServer(const NetSocketPair& pair)
{
    m_pSlave->createListenServer(pair);
}

void Process::setSignalHandlers()
{
    std::vector<Signal> signals = {
        Signal(SIGINT, "SIGINT", "kill all", childSignalHandler),
        Signal(SIGTERM, "SIGTERM", "kill softly", childSignalHandler),
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
    m_pEventLoop->runEvery(1, boost::bind(&test_child_period_read_sharememory, this));

    m_nStatus = true;

    while (m_nStatus) {
        m_pSlave->work();

        if (status_terminate || status_quit_softly || status_restart || status_reconfigure)
            m_nStatus = false;
    }
}

pid_t Process::getPid()
{
    return m_nPid;
}

bool Process::started()
{
    return m_nStatus;
}

int Process::join()
{
    // TODO:
    return 0;
}

    void Process::writeToShareMemory(const std::string& data)
    {
        void* address = nullptr;
        m_pSync->sem->lock();
        address = m_pSync->memory->getSpace();
        memcpy(address, (const void*)data.c_str(), data.size());
        m_pSync->sem->unLock();
    }

    std::string Process::readFromSharedMemory(void)
    {
        char* address = nullptr;
        unsigned int len = 0;
        std::string res;

        m_pSync->sem->lock();
        address = (char*)m_pSync->memory->getSpace();

        len = strlen(address);
        res.reserve(len);
        res.assign((const char*)address, len);

        m_pSync->sem->unLock();

        return res;
    }

Process::~Process()
{
    m_nPipe.clearSocket();
    delete m_pSlave;
    delete m_pEventLoop;
    LOG(Debug) << "class Process destructor\n";
}
