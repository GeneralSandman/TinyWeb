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

#ifndef __MUTEX_H
#define __MUTEX_H

#include <boost/noncopyable.hpp>
#include <pthread.h>

#include <tiny_base/api.h>

class MutexLock : boost::noncopyable
{
    private:
        pthread_mutex_t m_nMutex;
        pid_t m_nHolder;

    public:
        MutexLock()
        {
            m_nHolder = 0;
            int res = pthread_mutex_init(&m_nMutex, NULL);
            if (res != 0)
                handle_error("pthread_mutex_init() error");
        }
        void lock()
        {
            pthread_mutex_lock(&m_nMutex);
            // m_nHolder=currtid;
            //don't finished
        }
        void unlock()
        {
            m_nHolder = 0;
            pthread_mutex_unlock(&m_nMutex);
        }
        bool isLockByThisThread()
        {
            // return m_nHolder==currtid;
            //don't finished
            return true;
        }
        pthread_mutex_t* getPthreadMutex(){
            return &m_nMutex;
        }
        ~MutexLock()
        {
            //at this time,m_nHolder==0,
            int res = pthread_mutex_destroy(&m_nMutex);
            if (res != 0)
                handle_error("pthread_mutex_destroy() error");
        }
};

class MutexLockGuard : boost::noncopyable
{
    private:
        MutexLock &m_nMutexLock;
        //but the muduo is the reference,why?
        //MutexLock& mutex_;
    public:
        explicit MutexLockGuard(MutexLock &mutex)
            : m_nMutexLock(mutex)
        {
            m_nMutexLock.lock();
        }
        ~MutexLockGuard()
        {
            m_nMutexLock.unlock();
        }
};

#endif
