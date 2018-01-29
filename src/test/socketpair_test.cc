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

#include <tiny_core/socketpair.h>
#include <tiny_core/eventloop.h>
#include <tiny_core/connection.h>
#include <tiny_base/buffer.h>
#include <tiny_base/time.h>
#include <tiny_base/callback.h>

#include <iostream>
#include <unistd.h>
#include <boost/bind.hpp>

using namespace std;

void readCallback(Connection *con, Buffer *buf, Time time)
{
    std::cout << "read form\n";
}

void parent()
{
}

void child()
{
}

int main()
{
    SocketPair pipe;
    pipe.createSocket();

    pid_t pid = fork();

    if (pid < 0)
    {
        cout << "error\n";
    }
    else if (pid == 0)
    {
        EventLoop *loop = new EventLoop();
        pipe.setChildSocket(loop);
        pipe.setReadCallback(boost::bind(&readCallback, _1, _2, _3));

        loop->runAfter(5, boost::bind(&EventLoop::quit, loop));
        loop->loop();

        delete loop;
    }
    else
    {
        EventLoop *loop = new EventLoop();
        pipe.setParentSocket(loop);
        pipe.writeToChild("aa");

        loop->runAfter(10, boost::bind(&EventLoop::quit, loop));
        loop->loop();

        delete loop;
    }

    return 0;
}
