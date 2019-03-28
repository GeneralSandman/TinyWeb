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

#include <tiny_base/configer.h>
#include <tiny_base/log.h>
#include <tiny_core/eventloop.h>
#include <tiny_core/factory.h>
#include <tiny_core/master.h>
#include <tiny_core/netaddress.h>
#include <tiny_core/processpool.h>
#include <tiny_core/protocol.h>
#include <tiny_core/socket.h>

#include <unistd.h>
#include <vector>

extern int status_terminate;
extern int status_quit_softly;
extern int status_reconfigure;

Master::Master(ProcessPool* pool, EventLoop* loop, int num, const std::string& name)
    : m_pProcessPool(pool)
    , m_pEventLoop(loop)
    , m_nNumber(num)
    , m_nName(name)
{
    LOG(Debug) << "class Master constructor\n";
}

void Master::init()
{
    Configer& configer = Configer::getConfigerInstance();
    std::vector<ServerConfig> serverConf = configer.getServerConfig();

    for (auto t : serverConf) {
        std::cout << "listen port:" << t.listen << std::endl;
        NetAddress tmp("172.17.0.2", t.listen);
        Socket* socket = new Socket(createNoBlockSocket());
        socket->bindAddress(tmp);
        m_pListenSockets.push_back(socket);
        // FIXME:how to handle the problem of same listen-address.

        LOG(Info) << "bind address(" << tmp.getIpPort() << ")\n";
    }
}

void Master::getListenSockets(std::vector<int>& result)
{
    result.clear();
    for (auto socket : m_pListenSockets) {
        result.push_back(socket->getFd());
    }
}

void Master::work()
{
    LOG(Debug) << "[master]:start working\n";
    m_nStart = true;

    while (m_nStart) {
        m_pEventLoop->loop();
        if (status_terminate || status_quit_softly) {
            LOG(Debug) << "[master]:(term/stop)I will kill all chilern\n";
            m_pProcessPool->killAll();
            m_nStart = false;
        }
        if (status_restart || status_reconfigure) {
            LOG(Debug) << "[master]:(restart/reload)quit and create new processpool\n";
            m_pProcessPool->killAll();
            m_nStart = false;
        }
    }
}

Master::~Master()
{
    for (auto t : m_pListenSockets) {
        delete t;
    }
    LOG(Debug) << "class Master destructor\n";
}
