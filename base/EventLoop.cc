#include "EventLoop.h"
#include <iostream>
#include <poll.h>
#include <assert.h>

namespace muduo
{

EventLoop *t_loopInThisThread = nullptr;

bool EventLoop::m_fIsInLoopThread() const
{
    return pthread_equal(m_nThreadId,pthread_self());
}

void EventLoop::m_fAbortNotInLoopThread()
{
    std::cout << "EventLoop::abortNotInLoopThread\n";
}

void EventLoop::m_fAssertInLoopThread()
{
    if (!m_fIsInLoopThread())
    {
        m_fAbortNotInLoopThread();
    }
}

EventLoop::EventLoop(pid_t pid) : m_nLooping(false),
                                  m_nThreadId(pid)
{
    if (t_loopInThisThread)
    {
        std::cout << "have EventLoop in this thread\n";
    }
    else
    {
        t_loopInThisThread = this;
    }
}

EventLoop::~EventLoop()
{
    assert(!m_nLooping);
    t_loopInThisThread = nullptr;
}

void EventLoop::loop(void)
{
    assert(!m_nLooping);
    //m_fAssertInLoopThread();

    m_nLooping = true;
    std::cout<<"start loop\n";
    poll(nullptr, 0, 5 * 1000);
    m_nLooping = false;
    std::cout<<"stop loop\n";
}
}