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

#include <tiny_core/server.h>
#include <tiny_core/eventloop.h>
#include <tiny_core/netaddress.h>
#include <tiny_core/connection.h>
#include <tiny_core/protocol.h>
#include <tiny_core/factory.h>
#include <tiny_base/configer.h>
#include <tiny_base/buffer.h>
#include <tiny_core/time.h>
#include <tiny_base/api.h>
#include <tiny_base/log.h>

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <boost/bind.hpp>

using namespace std;

EventLoop *g_loop = nullptr;

void fun1()
{
    Time time(Time::now());
    cout << "invoke per second:" << time.toFormattedString() << endl;
}

void timeout()
{
    cout << "Timeout!\n";
    g_loop->quit();
}

static void signal_handler(int sig)
{
    std::cout << "receive signal:" << sig << std::endl;
    g_loop->quit();
}

int main()
{

    //config
    std::string configeFile = "/home/li/TinyWeb/TinyWeb.conf";
    setConfigerFile(configeFile);
    if (!loadConfig())
        std::cout << "load config failed\n";

    //log
    std::string loglevel = getConfigValue("loglevel");
    std::string logpath = getConfigValue("logpath");
    std::string debugfile = logpath + getConfigValue("debugfile");
    std::string infofile = logpath + getConfigValue("infofile");
    std::string warnfile = logpath + getConfigValue("warnfile");
    std::string errorfile = logpath + getConfigValue("errorfile");
    std::string fatalfile = logpath + getConfigValue("fatalfile");

    // initLogger(debugfile,
    //            infofile,
    //            warnfile,
    //            errorfile,
    //            fatalfile,
    //            convertStringToLoglevel(loglevel)); //error used

    //signal

    setLogLevel(Debug);

    add_signal(SIGTERM, signal_handler);
    add_signal(SIGINT, signal_handler);

    g_loop = new EventLoop();
    g_loop->runEvery(1, boost::bind(fun1));
    // g_loop->runAfter(60, boost::bind(timeout));

    // int port = atoi(getConfigValue("listen").c_str());
    NetAddress address("127.0.0.1:80");

    Protocol *prot = new WebProtocol();
    Factory *factory = new Factory(g_loop, prot);
    Server server(g_loop, address, factory);

    server.start();

    g_loop->loop();
    delete g_loop;
    delete factory;
    delete prot;
    return 0;
}