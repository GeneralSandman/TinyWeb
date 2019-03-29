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

#include <tiny_core/eventloop.h>
#include <tiny_core/factory.h>
#include <tiny_core/netaddress.h>
#include <tiny_core/protocol.h>
#include <tiny_core/server.h>
#include <tiny_core/slave.h>
#include <tiny_http/http_protocol.h>

#include <sys/types.h>
#include <unistd.h>

Slave::Slave(EventLoop* loop, int num, const std::string& name)
    : m_pEventLoop(loop)
    , m_nNumber(num)
    , m_nName(name)
    // , m_nListenAddress(NetAddress("172.17.0.2:9090"))
{
    LOG(Debug) << "class Slave constructor\n";
}

void Slave::createListenServer(const NetSocketPair& pair)
{
    Protocol* protocol = new WebProtocol();
    Factory* factory = new Factory(m_pEventLoop, protocol);
    Server* server = new Server(m_pEventLoop, pair.first,
        pair.second->getFd(), factory);
    VritualMachine newMachine(protocol, factory,
        pair.second->getFd(), server);

    m_nMachines.push_back(newMachine);
}

#include <tiny_core/status.h>
extern int status_quit_softly; //QUIT
extern int status_terminate;   //TERM,INT
extern int status_exiting;
extern int status_reconfigure; //HUP,reboot
extern int status_child_quit;  //CHLD

void Slave::work()
{
    for (auto t : m_nMachines) {
        t.m_pServer->start();
    }
    m_nStart = true;

    while (m_nStart) {
        m_pEventLoop->loop();
        // reconfig

        if (status_terminate || status_quit_softly || status_restart || status_reconfigure)
            m_nStart = false;
    }
}

Slave::~Slave()
{
    for (auto t : m_nMachines) {
        delete t.m_pProtocol;
        delete t.m_pFactory;
        delete t.m_pServer;
    }
    LOG(Debug) << "class Slave destructor\n";
}
