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

#include <tiny_core/netaddress.h>

#include <string>

class EventLoop;
class Protocol;
class Factory;
class Server;

class Slave
{
    protected:
        EventLoop *m_pEventLoop;
        int m_nNumber;
        std::string m_nName;

        Protocol *m_pProtocol;
        Factory *m_pFactory;

        NetAddress m_nListenAddress;
        int m_nListenSocketFd;
        Server *m_pServer;

        int status;

    public:
        Slave(EventLoop *, int, const std::string &);
        void createListenServer(int listenSocket);
        void work();
        ~Slave();
};

#endif //!SLAVE_H
