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
#include <tiny_core/eventloop.h>
#include <tiny_core/master.h>
#include <tiny_core/netaddress.h>
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
      m_nName(name)
{
    LOG(Debug) << "class Master constuctor\n";
}

void Master::init()
{
    m_nListenSocket = createNoBlockSocket();
}

int Master::getListenSocket()
{
    return m_nListenSocket;
}

void Master::work()
{
    m_pEventLoop->loop();
    if (status_terminate || status_quit_softly)
    {
        std::cout << "[master]:I will kill all chilern\n";
        m_pProcessPool->killAll();
    }
    if (status_reconfigure)
    {
        std::cout << "[master]:reconfigure ,recreate new process\n";
        m_pProcessPool->killAll();
        m_pProcessPool->createProcess(m_pProcessPool->processNum());
    }
}

Master::~Master()
{
    LOG(Debug) << "class Master destuctor\n";
}
