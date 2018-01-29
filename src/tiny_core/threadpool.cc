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


#include <tiny_core/thread.h>
#include <tiny_core/threadpool.h>
#include <tiny_core/currentthread.h>
#include <tiny_base/mutex.h>
#include <tiny_base/condition.h>

#include <boost/bind.hpp>

void ThreadPool::m_fRunInThread()
{
    //need handle exception
    while (m_nRunning)
    {
        Task t = m_fTake();
        if (t)
            t();
    }
}

Task ThreadPool::m_fTake()
{
    MutexLockGuard lock(m_nMutexLock);
    while (m_nQueue.empty() && m_nRunning)
    {
        m_nCondition_notEmpty.wait();
    }
    Task t;
    // always use a while-loop, due to spurious wakeup
    if (!m_nQueue.empty())
    {
        t = m_nQueue.front();
        m_nQueue.pop_front();
        m_nCondition_notFull.notify();
    }
    return t;
    // t may be nullptr
}

bool ThreadPool::m_fIsFull()
{
    return m_nQueue.size() == m_nMaxQueueSize;
}

ThreadPool::ThreadPool(const std::string &n,
                       int maxqueuesize)
    : m_nName(n),
      m_nCondition_notEmpty(m_nMutexLock),
      m_nCondition_notFull(m_nMutexLock),
      m_nMaxQueueSize(maxqueuesize),
      m_nRunning(false)
{
    //m_nQueue init
    //m_nThreads init
}
void ThreadPool::start(int n)
{
    //create n threads
    m_nThreads.reserve(n);

    for (int i = 0; i < n; i++)
    {
        char id[32];
        snprintf(id, sizeof id, "%d", i);
        std::string newName = m_nName + id;
        Thread *newThread = new Thread(
            boost::bind(&ThreadPool::m_fRunInThread, this),
            newName);
        newThread->start();
        m_nThreads.push_back(newThread);
        //why use this*
        //https://kelvinh.github.io/blog/2013/12/03/boost-bind-illustrated/
        //http://www.jianshu.com/p/30e79d2a8f7d
    }
    m_nRunning = true;
}
void ThreadPool::stop()
{
    //
    {
        MutexLockGuard lock(m_nMutexLock);
        m_nRunning = false;
        //what should we do?
        //don't finished
        //not like muduo ,think why
        m_nCondition_notEmpty.notifyAll();
        m_nCondition_notFull.notifyAll();
        //weakup all threads which is wait()
    }
    for_each(m_nThreads.begin(),
             m_nThreads.end(),
             boost::bind(&Thread::join, _1));
}
bool ThreadPool::run(const Task &t)
{
    //if task not been pushed or not been invoked
    //return false

    //if no child thread
    //main thread will do task
    //else:push
    if (m_nThreads.empty())
    {
        t();
        return true;
    }
    else
    {
        MutexLockGuard lock(m_nMutexLock);
        while (m_fIsFull() && m_nRunning)
        {
            m_nCondition_notFull.wait();
        }

        if (!m_fIsFull())
        {
            m_nQueue.push_back(t);
            m_nCondition_notEmpty.notify();
            return true;
        }
    }

    return false;
}
ThreadPool::~ThreadPool()
{
    if (m_nRunning)
        stop();
}
