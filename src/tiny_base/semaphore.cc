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

#include <tiny_base/semaphore.h>
#include <tiny_base/log.h>

#include <semaphore.h>

Semaphore::Semaphore(int value)
{
    sem_init(&m_nSem, 1, value);
    //The semaphore is shared between process
    LOG(Debug) << "class Semaphore constructor\n";
}

void Semaphore::wait()
{
    sem_wait(&m_nSem);
}

void Semaphore::tryWait()
{
    sem_trywait(&m_nSem);
}

void Semaphore::post()
{
    sem_post(&m_nSem);
}

int Semaphore::getValue()
{
    int res;
    sem_getvalue(&m_nSem, &res);
    return res;
}

Semaphore::~Semaphore()
{
    sem_destroy(&m_nSem);
    LOG(Debug) << "class Semaphore destructor\n";
}
