/*
 *Author:GeneralSandman
 *Code:https://github.com/GeneralSandman/TinyWeb
 *E-mail:generalsandman@163.com
 *Web:www.dissigil.cn
 */

/*---class Proxyer---
 *
 ****************************************
 *
 */

#ifndef PROXYER_H
#define PROXYER_H

#include <tiny_core/netaddress.h>
#include <tiny_core/connection.h>
#include <tiny_core/server.h>
#include <tiny_core/client.h>

#include <map>

//first:(as a server)
//  accept tcp connection,
//  get http request message.
//second:(as a client)
//  connect server in according of http request,
//  send http request.
//third：(as a server)
//  get http response,
//  write http response to client.

class EventLoop;
class Factory;

class Proxyer
{
    private:
        // std::map<NetAddress, Connection *> m_nServers;
        // std::map<NetAddress, Connection *> m_nClients;

        // Server m_nServer;
        // Client m_nClient;
        // EventLoop *m_pEventLoop;
        // bool m_nStarted;
        ConnectionCallback m_nConnectCallback;
        MessageCallback m_nMessageCallback;
        WriteCompleteCallback m_nWriteCompleteCallback;
        CloseCallback m_nCloseCallback;
        // Factory *m_pFactory;

    public:
        Proxyer();
        void start();
        ~Proxyer();
};

#endif // !PROXYER_H
