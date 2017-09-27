#include "../server.h"
#include "../eventloop.h"
#include "../netaddress.h"
#include "../time.h"
#include "../api.h"
#include "../log.h"

#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <boost/bind.hpp>

using namespace std;
EventLoop *g_loop = nullptr;

void madeConnection()
{
    cout << "+++new Connection+++" << endl;
}

void getMessage()
{
    cout << "+++get message+++\n";
}

void lostConnection()
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
    setLogLevel(Info);

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