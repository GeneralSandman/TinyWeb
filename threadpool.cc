#include "thread.h"
#include "threadpool.h"
#include "currentthread.h"
#include "condition.h"

#include <boost/bind.hpp>

void ThreadPool::m_fRunInThread() {}
Task ThreadPool::m_fTake() {}

ThreadPool::ThreadPool(const std::string &n)
    : m_nName(n),
      m_nCondition_notEmpty(m_nMutexLock),
      m_nCondition_notFull(m_nMutexLock),
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
void ThreadPool::stop() {}
void ThreadPool::run(const Task &) {}
ThreadPool::~ThreadPool() {}
