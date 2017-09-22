#include <mqueue.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "../api.h"

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

mqd_t mqd;

void create()
{

    int c, flags;

    flags = O_RDWR | O_CREAT | O_EXCL;
    // /dev/mqueue/
    mqd = mq_open("/mq123", flags, FILE_MODE, NULL);
    if (mqd == -1)
    {
        handle_error("mq_open error:");
    }

    mq_close(mqd);
}

void remove()
{
    mq_unlink("/mq123");
}

void send()
{

    mqd_t mqd;
    char ptr[] = "abc";
    size_t len;
    unsigned prio;

    len = 3;
    prio = 1;
    mqd = mq_open("/mq123", O_WRONLY);

    mq_send(mqd, (const char *)ptr, len, prio);
}

void recv()
{

    mqd_t mqd;
    void *ptr;
    size_t len;
    unsigned prio;

    len = 3;
    prio = 1;
    mqd = mq_open("/mq123", O_RDONLY);

    struct mq_attr attr;
    mq_getattr(mqd, &attr);
    ptr = malloc(attr.mq_msgsize);

    mq_receive(mqd, (char *)ptr, attr.mq_msgsize, &prio);
    std::cout << (char *)ptr << std::endl;
}

int main(int argc, char **argv)
{
    using namespace std;
    int i;
    cin >> i;

    if (i == 1)
    {
        create();
    }
    else if (i == 2)
    {
        remove();
    }
    else if (i == 3)
    {
        send();
    }
    else if (i == 4)
    {
        recv();
    }

    return 0;
}