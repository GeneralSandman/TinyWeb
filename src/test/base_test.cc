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

#include <tiny_core/eventloop.h>
#include <tiny_core/channel.h>

#include <iostream>
#include <sys/timerfd.h>
#include <boost/bind.hpp>

using namespace std;

EventLoop *g_loop = nullptr;

void timeout()
{
    cout << "Timeout!\n";
    g_loop->quit();
}

void fun1()
{
    cout << "invoke per second\n";
}

void fun2()
{
    cout << "invoke per 2 seconds\n";
}

void fun3()
{
    cout << "invoke per 3 seconds\n";
}

int main()
{

    g_loop = new EventLoop();

    g_loop->runEvery(1, boost::bind(fun1));
    g_loop->runEvery(2, boost::bind(fun2));
    g_loop->runEvery(3, boost::bind(fun3));
    g_loop->runAfter(10, boost::bind(timeout));

    g_loop->loop();
    delete g_loop;
    return 0;
}