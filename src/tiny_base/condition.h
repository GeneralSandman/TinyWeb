/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*---Configer Class---
*We need to upgrade this class
*in order to handle config file error
****************************************
*
*/

#ifndef CONDITION_H
#define CONDITION_H

#include <tiny_base/api.h>
#include <tiny_base/mutex.h>

#include <boost/noncopyable.hpp>
#include <pthread.h>

class Condition : boost::noncopyable
{
  private:
    MutexLock &m_nMutexLock;
    pthread_cond_t m_nCond;

  public:
    explicit Condition(MutexLock &mutex)
        : m_nMutexLock(mutex)
    {
        int res = pthread_cond_init(&m_nCond, NULL);
        if (res != 0)
            handle_error("pthread_cond_init() error");
    }
    void wait()
    {
        pthread_cond_wait(&m_nCond,
                          m_nMutexLock.getPthreadMutex());
    }
    bool waitForSeconds(int second)
    {
        //if time out return true,
        struct timespec abs;
        clock_gettime(CLOCK_REALTIME, &abs);
        abs.tv_sec += second;
        return ETIMEDOUT == pthread_cond_timedwait(&m_nCond,
                                                   m_nMutexLock.getPthreadMutex(),
                                                   &abs);
    }
    void notify()
    {
        pthread_cond_signal(&m_nCond);
    }
    void notifyAll()
    {
        pthread_cond_broadcast(&m_nCond);
    }
    ~Condition()
    {
        int res = pthread_cond_destroy(&m_nCond);
        if (res != 0)
            handle_error("pthread_cond_destroy() error");
    }
};

#endif