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
#include <tiny_core/time.h>
#include <tiny_core/timerid.h>
#include <tiny_core/callback.h>

#include <iostream>
#include <unistd.h>
#include <boost/bind.hpp>

using namespace std;

void getMessage(Connection *con, Buffer *buf, Time time)
{
    std::cout << "receve data:" << buf->getAll() << std::endl;
}

void print()
{
    // std::cout << getpid() << "--\n";
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
        pipe.setMessageCallback(boost::bind(&getMessage, _1, _2, _3));

        TimerId id1 = loop->runEvery(1, boost::bind(&SocketPair::writeToParent, &pipe, "bb"));
        TimerId id2 = loop->runAfter(10, boost::bind(&EventLoop::quit, loop));
        loop->loop();

        pipe.clearSocket();
        delete loop;
    }
    else
    {
        EventLoop *loop = new EventLoop();
        pipe.setParentSocket(loop);
        pipe.setMessageCallback(boost::bind(&getMessage, _1, _2, _3));

        TimerId id1 = loop->runEvery(1, boost::bind(&SocketPair::writeToChild, &pipe, "aa"));
        TimerId id2 = loop->runAfter(10, boost::bind(&EventLoop::quit, loop));
        loop->loop();

        pipe.clearSocket();
        delete loop;
    }

    return 0;
}
