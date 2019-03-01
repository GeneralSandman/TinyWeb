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

#include <tiny_base/sync.h>
#include <tiny_core/time.h>

#include <iostream>
#include <memory> 
#include <string.h>
#include <unistd.h>

using namespace std;

void consumer(Sync* sync)
{
    int* data;
    std::string str;
    for (int i = 0; i < 1; i++) {
        sync->sem->lock();
        data = (int*)sync->memory->getSpace();
        std::cout << "consumer:" << *data << std::endl;
        sync->sem->unLock();
    }
}

void productor(Sync* sync)
{
    int* data;
    std::string str;
    for (int i = 0; i < 20; i++) {
        sync->sem->lock();
        data = (int*)sync->memory->getSpace();
        *data = *data + 1;
        std::cout << "[" << getpid() << "]write:" << *data << std::endl;
        sync->sem->unLock();
    }
}

int main()
{
    shared_ptr<Sync> sync = std::make_shared<Sync>(sizeof(int));

    int* data = (int*)sync->memory->getSpace();
    *data = 0;

    pid_t pid;
    pid = fork();
    if (pid < 0) {
        std::cout << "fork error" << std::endl;
        return -1;
    } else if (pid == 0) {
        sleep(15);
        consumer(sync.get());
    } else {

        pid = fork();
        if (pid < 0) {
            std::cout << "fork2 error" << std::endl;
            return -1;
        } else if (pid == 0) {
            productor(sync.get());
        } else {
            productor(sync.get());
        }
    }
    std::cout << "exit" << std::endl;

    return 0;
}
