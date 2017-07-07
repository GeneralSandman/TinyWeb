#include "EventLoop.h"
#include <pthread.h>
#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

using namespace muduo;

void *threadFunc(void *argc)
{
    printf("threadFunc(): pid=%lu tid=%lu",
           (unsigned long)getpid(),
           (unsigned long)pthread_self());
}

int main()
{
    printf("main(): pid=%lu tid=%lu",
           (unsigned long)getpid(),
           (unsigned long)pthread_self());
    EventLoop *loop = new EventLoop(pthread_self());

    pthread_t tid;
    int res=pthread_create(&tid,NULL,threadFunc,NULL);
    loop->loop();
    
    delete loop;
}