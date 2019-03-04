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

#include <tiny_base/sharedmemory.h>
#include <tiny_base/semaphore.h>
#include <tiny_base/log.h>
#include <tiny_base/api.h> 

#include <semaphore.h>

Semaphore::Semaphore(SharedMemory *memory, int value)
{
    m_nSem = (sem_t *)memory->getSpace();
    sem_init(m_nSem, 1, value); // shared in multi-process.
    LOG(Debug) << "class Semaphore constructor\n";
}

void Semaphore::lock()
{
    int res;
    res = sem_wait(m_nSem);

    if (res == -1) {
        handle_error("sem_wait() error:");
    }
}

void Semaphore::tryLock()
{
    // FIXME:
    int res;
    res = sem_trywait(m_nSem);

    if (res == -1) {
        handle_error("sem_wait() error:");
    }
}

void Semaphore::unLock()
{
    int res;
    res = sem_post(m_nSem);

    if (res == -1) {
        handle_error("sem_wait() error:");
    }
}

int Semaphore::getValue()
{
    int res;
    sem_getvalue(m_nSem, &res);
    return res;
}

Semaphore::~Semaphore()
{
    sem_destroy(m_nSem);
    LOG(Debug) << "class Semaphore destructor\n";
}
