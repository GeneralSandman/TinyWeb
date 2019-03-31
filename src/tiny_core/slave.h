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

#ifndef SLAVE_H
#define SLAVE_H

#include <tiny_base/log.h>
#include <tiny_core/netaddress.h>

#include <string>
#include <vector>

class EventLoop;
class Protocol;
class Factory;
class Server;

class VritualMachine {
public:
    Protocol* m_pProtocol;
    Factory* m_pFactory;
    int m_nListenSocketFd;
    Server* m_pServer;

    VritualMachine(Protocol* protocol,
        Factory* factory,
        int listenFd,
        Server* server)
        : m_pProtocol(protocol)
        , m_pFactory(factory)
        , m_nListenSocketFd(listenFd)
        , m_pServer(server)
    {
        LOG(Debug) << "class VritualMachine constructor\n";
    }
};

class Slave {
protected:
    EventLoop* m_pEventLoop;
    int m_nNumber;
    std::string m_nName;

    // NetAddress m_nListenAddress; // TODO:delete

    std::vector<VritualMachine> m_nMachines;

    bool m_nStart;

    typedef std::pair<NetAddress, Socket*> NetSocketPair;
public:
    Slave(EventLoop*, int, const std::string&);
    void createListenServer(const NetSocketPair& pair);
    void work();
    ~Slave();
};

#endif //!SLAVE_H
