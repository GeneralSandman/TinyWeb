#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include <string>
#include <deque>
#include "mutex.h"
#include "condition.h"

#include "thread.h"

typedef boost::function<void()> Task;

class ThreadPool : boost::noncopyable
{
  private:
    std::string m_nName;
    MutexLock m_nMutexLock;
    Condition m_nCondition_notEmpty;
    Condition m_nCondition_notFull;
    std::deque<Task> m_nQueue;
    boost::ptr_vector<Thread> m_nThreads;
    bool m_nRunning;

    void m_fRunInThread();
    Task m_fTake();

  public:
    //constructer don't create thread
    //start(n) create n theads
    explicit ThreadPool(const std::string &a = "");
    void start(int);
    void stop();
    void run(const Task &);
    ~ThreadPool();
};

#endif // !THREAD_POOL_H
