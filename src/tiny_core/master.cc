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
#include <tiny_core/eventloop.h>
#include <tiny_core/master.h>
#include <tiny_core/netaddress.h>
#include <tiny_core/socket.h>
#include <tiny_core/protocol.h>
#include <tiny_core/factory.h>
#include <tiny_base/log.h>

#include <unistd.h>

extern int status_terminate;
extern int status_quit_softly;
extern int status_reconfigure;

Master::Master(ProcessPool *pool, EventLoop *loop, int num, const std::string &name)
    : m_pProcessPool(pool),
    m_pEventLoop(loop),
    m_nNumber(num),
    m_nName(name),
    m_pListenSocket(new Socket(createNoBlockSocket()))
{
    LOG(Debug) << "class Master constuctor\n";
}

void Master::init()
{
    NetAddress tmp(9999);
    m_pListenSocket->bindAddress(tmp);
}

int Master::getListenSocket()
{
    return m_pListenSocket->getFd();
}

void Master::work()
{
    LOG(Debug) << "[master]:start working\n";
    status = 1;

    while (status)
    {
        m_pEventLoop->loop();
        if (status_terminate || status_quit_softly)
        {
            LOG(Debug) << "[master]:I will kill all chilern\n";
            m_pProcessPool->killAll();
            status = 0;
        }
        if (status_reconfigure)
        {
            LOG(Debug) << "[master]:reconfigure ,recreate new process\n";
            m_pProcessPool->killAll();
            m_pProcessPool->createProcess(m_pProcessPool->processNum());
        }

    }
}

Master::~Master()
{
    LOG(Debug) << "class Master destuctor\n";
}
