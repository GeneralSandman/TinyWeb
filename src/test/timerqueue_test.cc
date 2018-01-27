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

#include "../timerqueue.h"
#include "../timerid.h"
#include "../eventloop.h"

#include <iostream>
#include <boost/bind.hpp>

using namespace std;

EventLoop *g_loop = nullptr;

int global_num1 = 0;
int global_num2 = 0;

TimerId timerid1, timerid2;

void fun1()
{
    cout << "[Timer1]:cout per second\n";
    global_num1++;
    if (global_num1 == 10)
    {
        cout << "[Timer1]:cancel Timer1\n";
        g_loop->cancelTimerId(timerid1);
    }
}

void fun2()
{
    cout << "[Timer2]:cout per two second\n";
    global_num2++;
    if (global_num2 == 5)
    {
        cout << "[Timer2]:cancel Timer2\n";
        g_loop->cancelTimerId(timerid2);
    }
}

void timeout()
{
    cout << "[Timer3]:13 seconds from main function started\n";
    cout << "[Timer3]:quit eventloop\n";
    g_loop->quit();
}

int main()
{
    g_loop = new EventLoop();
    timerid1 = g_loop->runEvery(1, boost::bind(fun1));
    timerid2 = g_loop->runEvery(2, boost::bind(fun2));

    if (timerid1.isVaild())
        std::cout << "timerid1 vaild\n";
    else
        std::cout << "timerid1 invaild\n";

    if (timerid2.isVaild())
        std::cout << "timerid2 vaild\n";
    else
        std::cout << "timerid2 invaild\n";

    std::cout << "after cancel timerid1 and timerid2\n";
    g_loop->cancelTimerId(timerid1);
    g_loop->cancelTimerId(timerid2);
    
    if (timerid1.isVaild())
        std::cout << "timerid1 vaild\n";
    else
        std::cout << "timerid1 invaild\n";

    if (timerid2.isVaild())
        std::cout << "timerid2 vaild\n";
    else
        std::cout << "timerid2 invaild\n";

    g_loop->runAfter(13, boost::bind(timeout));

    g_loop->loop();
    delete g_loop;
    return 0;
}