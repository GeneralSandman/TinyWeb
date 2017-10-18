/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

#include "../server.h"
#include "../eventloop.h"
#include "../netaddress.h"
#include "../connection.h"
#include "../protocol.h"
#include "../factory.h"
#include "../configer.h"
#include "../buffer.h"
#include "../time.h"
#include "../api.h"
#include "../log.h"

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <boost/bind.hpp>

using namespace std;

EventLoop *g_loop = nullptr;

void madeConnection(Connection *con)
{
    cout << "+++new Connection+++" << endl;
    string msg = "I have get you connect\n";
    // con->send(msg);
}

void getMessage(Connection *con,
                Buffer *input,
                Time arriveTime)
{
    cout << "+++get message+++:";
    cout << input->getAll() << std::endl;
    string msg = "I am server:I have get you message";
    con->send(msg);
    // con->shutdownWrite();//error
}

void lostConnection(Connection *con)
{
    cout << "+++lost Connection+++\n";
}

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
    // std::string configeFile = "/home/li/TinyWeb/TinyWeb.conf";
    // setConfigerFile(configeFile);
    // if (!loadConfig())
    //     std::cout << "load config failly\n";

    //log
    // std::string loglevel = getConfigValue("loglevel");
    // std::string logpath = getConfigValue("logpath");
    // std::string debugfile = logpath + getConfigValue("debugfile");
    // std::string infofile = logpath + getConfigValue("infofile");
    // std::string warnfile = logpath + getConfigValue("warnfile");
    // std::string errorfile = logpath + getConfigValue("errorfile");
    // std::string fatalfile = logpath + getConfigValue("fatalfile");

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
    g_loop->runAfter(60, boost::bind(timeout));

    // int port = atoi(getConfigValue("listen").c_str());
    NetAddress address("127.0.0.1:9898");

    NewProtocol prot;
    Factory *factory = new Factory(prot);
    Server server(g_loop, address, factory);

    // server.setConenctCallback(madeConnection);
    // server.setMessageCallback(getMessage);
    // server.setCloseCallback(lostConnection);
    server.start();

    g_loop->loop();
    delete g_loop;
    delete factory;
    return 0;
}