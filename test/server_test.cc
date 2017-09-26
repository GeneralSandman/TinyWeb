#include "../server.h"
#include "../eventloop.h"
#include "../netaddress.h"
#include "../time.h"
#include "../api.h"

#include <iostream>
#include <unistd.h>
#include <boost/bind.hpp>

using namespace std;
EventLoop *g_loop = nullptr;

void ConnectionCallback()
{
    cout << "new Connection:" << endl;
}

void MessageCallback()
{
    cout << "get message\n";
}

void fun1()
{
    Time time(Time::now());
    cout << "invoke per second:" << time.toString() << endl;
}

void timeout()
{
    cout << "Timeout!\n";
    g_loop->quit();
}

int main()
{

    g_loop = new EventLoop();
    g_loop->runEvery(1, boost::bind(fun1));
    g_loop->runAfter(100, boost::bind(timeout));

    NetAddress address("127.0.0.1:9898");
    Server server(g_loop, address);
    server.setMessageCallback(MessageCallback);
    server.setConenctCallback(ConnectionCallback);
    server.start();

    g_loop->loop();
    delete g_loop;
    return 0;
}