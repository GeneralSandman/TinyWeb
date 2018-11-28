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

#include <tiny_base/configer.h>
#include <tiny_base/buffer.h>
#include <tiny_base/api.h>
#include <tiny_base/log.h>
#include <tiny_core/server.h>
#include <tiny_core/eventloop.h>
#include <tiny_core/netaddress.h>
#include <tiny_core/connection.h>
#include <tiny_core/protocol.h>
#include <tiny_core/factory.h>
#include <tiny_core/time.h>
#include <tiny_core/timerid.h>
#include <tiny_http/http_factory.h>
#include <tiny_http/http_protocol.h>
#include <tiny_http/http_parser.h>

#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

EventLoop *g_loop = nullptr;

static void m_fSignalHandler(int sig)
{
    if (g_loop)
        g_loop->quit();
}

void m_fInit()
{
    //first step:
    //add signal handler
    add_signal(SIGTERM, m_fSignalHandler);
    add_signal(SIGINT, m_fSignalHandler);
}

void fun1()
{
    LOG(Info) << "info-\n";
}

int main()
{
    //signal
    headerMeaningInit(); //TODO:delete

    add_signal(SIGTERM, m_fSignalHandler);
    add_signal(SIGINT, m_fSignalHandler);

    g_loop = new EventLoop();
    g_loop->runEvery(1, boost::bind(fun1));

    int listenFd = createNoBlockSocket();
    NetAddress address("127.0.0.1:9090");
    struct sockaddr_in s = address.getAddr();
    Bind(listenFd, &s);

    WebProtocol prot;
    Factory *factory = new Factory(g_loop, &prot);
    Server server(g_loop, address, listenFd, factory);

    server.start();

    g_loop->loop();
    delete g_loop;
    delete factory;
    return 0;
}