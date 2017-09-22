#include <iostream>
#include <sys/timerfd.h>

#include "../eventloop.h"
#include "../channel.h"

using namespace std;

EventLoop *g_loop = nullptr;

void timeout()
{
    cout << "Timeout!\n";
    g_loop->quit();
}

int main()
{

    g_loop = new EventLoop();

    int timerfd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    Channel channel1(g_loop, timerfd);
    channel1.setReadCallback(timeout);
    channel1.enableRead();

    struct itimerspec howlong;
    bzero(&howlong, sizeof howlong);
    howlong.it_value.tv_sec = 5;
    ::timerfd_settime(timerfd, 0, &howlong, NULL);

    g_loop->loop();

    ::close(timerfd);
    delete g_loop;
    return 0;
}