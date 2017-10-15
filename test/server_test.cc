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
#include "../buffer.h"
#include "../time.h"
#include "../api.h"
#include "../log.h"

#include <iostream>
#include <unistd.h>
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
    setLogLevel(Debug);

    add_signal(SIGTERM, signal_handler);
    add_signal(SIGINT, signal_handler);

    g_loop = new EventLoop();
    g_loop->runEvery(1, boost::bind(fun1));
    g_loop->runAfter(60, boost::bind(timeout));

    NetAddress address("127.0.0.1:9898");
    Server server(g_loop, address);

    server.setConenctCallback(madeConnection);
    server.setMessageCallback(getMessage);
    server.setCloseCallback(lostConnection);
    server.start();

    g_loop->loop();
    delete g_loop;
    return 0;
}