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

#include <tiny_core/eventloop.h>
#include <tiny_core/master.h>
#include <tiny_core/netaddress.h>
#include <tiny_core/protocol.h>
#include <tiny_core/factory.h>
#include <tiny_base/log.h>

#include <unistd.h>

Master::Master(EventLoop *loop, int num, const std::string &name)
    : m_pEventLoop(loop),
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
}

Master::~Master()
{
    LOG(Debug) << "class Master destuctor\n";
}
