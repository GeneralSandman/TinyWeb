/*
 *Author:GeneralSandman
 *Code:https://github.com/GeneralSandman/TinyWeb
 *E-mail:generalsandman@163.com
 *Web:www.dissigil.cn
 */

/*---XXX---
 *
 ****************************************
 *
 */

#ifndef SYNC_H
#define SYNC_H

#include <tiny_base/semaphore.h>
#include <tiny_base/sharedmemory.h>

class Sync {
private:
    SharedMemory* sem_memory;
    // the space for semaphore to synchronize.

public:
    Semaphore* sem;
    SharedMemory* memory;

    Sync(unsigned int size)
    {
        sem_memory = new SharedMemory(sizeof(sem_t));
        sem = new Semaphore(sem_memory, 1);

        memory = new SharedMemory(size);
    };

    ~Sync()
    {
        delete memory;
        delete sem;
        delete sem_memory;
    };
};

#endif // !SYNC_H
