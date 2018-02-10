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
#include <tiny_core/currentthread.h>
#include <tiny_base/atomic.h>
#include <tiny_base/api.h>

AtomicInt32 Thread::m_nThreadsNum;

void *Thread::m_fStartThread(void *arg)
{
    Thread *thread = (Thread *)arg;
    thread->runInThread();
    return nullptr;
}

void Thread::runInThread()
{
    //invoke the thread_function
    m_nTid = CurrentThread::tid();
    CurrentThread::t_threadName = m_nName.c_str();
    m_fThreadFun();
    CurrentThread::t_threadName = "finished";
    //don't finised
    //must handle the exception
}

Thread::Thread(const threadFun &fun,
               const std::string &name)
{
    m_nName = name;
    m_nThreadId = 0;
    m_nTid = 0;
    m_fThreadFun = fun;
    m_nStarted = false;
    m_nJoined = false;
    m_nThreadsNum.increment();
}

void Thread::start()
{
    m_nStarted = true;
    int res = pthread_create(&m_nThreadId, NULL,
                             m_fStartThread, this);
    if (res != 0)
        handle_error_s("pthread_create() error");
}

int Thread::join()
{
    m_nJoined = true;
    return pthread_join(m_nThreadId, NULL);
}

bool Thread::started()
{
    return m_nStarted;
}

pid_t Thread::getTid()
{
    return m_nTid;
}

const std::string &Thread::getName()
{
    return m_nName;
}

int Thread::numCreated()
{
    return m_nThreadsNum.get();
}

Thread::~Thread()
{
    if (!m_nJoined)
        pthread_detach(m_nThreadId);
}