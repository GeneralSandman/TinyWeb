#include "../currentthread.h"
#include "../threadpool.h"
#include <stdio.h>
#include <string>
#include <boost/bind.hpp>
#include <unistd.h>
void print()
{
    printf("tid=%d\n", CurrentThread::tid());
}

void printString(const std::string &str)
{
    printf("tid=%d, str=%s\n", CurrentThread::tid(), str.c_str());
}

int main()
{
    printf("pid=%d, tid=%d\n", ::getpid(), CurrentThread::tid());

    ThreadPool pool("MainThreadPool");
    pool.start(4);

    pool.run(print);
    pool.run(print);
    for (int i = 0; i < 100; ++i)
    {
        char buf[32];
        snprintf(buf, sizeof buf, "task %d", i);
        pool.run(boost::bind(printString, std::string(buf)));
    }

    //    CountDownLatch latch(1);
    //   pool.run(boost::bind(& CountDownLatch::countDown, &latch));
    //   latch.wait();
    sleep(1);
    pool.stop();

    return 0;
}
