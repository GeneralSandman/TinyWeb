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

#ifndef THREAD_POOL_H
#define THREAD_POOL_H


#include <tiny_base/mutex.h>
#include <tiny_base/condition.h>
#include <tiny_core/thread.h>

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <string>
#include <deque>

typedef boost::function<void()> Task;

class ThreadPool : boost::noncopyable
{
    private:
        std::string m_nName;
        MutexLock m_nMutexLock;
        Condition m_nCondition_notEmpty;
        Condition m_nCondition_notFull;
        unsigned int m_nMaxQueueSize;
        std::deque<Task> m_nQueue;
        boost::ptr_vector<Thread> m_nThreads;
        bool m_nRunning;

        void m_fRunInThread();
        Task m_fTake();
        bool m_fIsFull();

    public:
        //constructer don't create thread
        //start(n) create n theads
        explicit ThreadPool(const std::string &a = "",
                int maxqueuesize = 100);
        void start(int);
        void stop();
        bool run(const Task &);
        ~ThreadPool();
};

#endif // !THREAD_POOL_H
