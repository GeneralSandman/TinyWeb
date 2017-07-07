#ifndef EVENTLOOP_H
#define EVENTLOOP_H
#include <pthread.h>

namespace muduo
{

class EventLoop
{
  private:
    const pid_t m_nThreadId;
    bool m_nLooping;

    bool m_fIsInLoopThread() const;
    void m_fAbortNotInLoopThread();
    void m_fAssertInLoopThread();

  public:
    EventLoop(pid_t);
    ~EventLoop();

    void loop(void);
};
}

#endif