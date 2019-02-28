/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*---class Semaphore---
*
****************************************
*
*Semaphore():create memory-based semaphore and initialize the value of
            semaphore to the pararm
*wait():wait semaphore(wait if the value is less than or equal to 0,and 
*       decrease the value the semaphore)
*tryWait():
*post():post to a semaphore(increase the value of the semaphore)
*getValue():
*~Semaphore():destory the semaphore
*/

#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <tiny_base/sharedmemory.h>

#include <semaphore.h>

class Semaphore
{
  private:
    sem_t* m_nSem;
    //sem_t must be stored in shared memory

  public:
    Semaphore(SharedMemory *memory, int value=0);
    void lock();
    void tryLock();
    void unLock();
    int getValue();
    ~Semaphore();
};

#endif // !SEMAPHORE_H
