#include "timerqueue.h"
#include "timer.h"
#include "channel.h"
#include "log.h"

#include <sys/timerfd.h>
#include <boost/bind.hpp>
#include <unistd.h>
#include <set>

struct timespec howMuchTimeFromNow(Time when)
{
    int64_t microseconds = when.microSecondsSinceEpoch() - Time::now().microSecondsSinceEpoch();
    if (microseconds < 100)
    {
        microseconds = 100;
    }
    struct timespec ts;
    ts.tv_sec = static_cast<time_t>(
        microseconds / Time::kMicroSecondsPerSecond);
    ts.tv_nsec = static_cast<long>(
        (microseconds % Time::kMicroSecondsPerSecond) * 1000);
    return ts;
}

void TimerQueue::m_fHandleRead()
{
    //get happened and invoke the timer's callback
    //if a timer will repet,reset
    //else delete this timer

    std::vector<Timer *> happened;
    m_fGetHappen(happened);
    for (auto t : happened)
        t->run();

    m_fResetHappened(happened);
}

void TimerQueue::m_fInsertTimer(Timer *timer)
{
    Time time = timer->getTime();
    m_nTimers.insert(std::pair<Time, Timer *>(time, timer));
}

void TimerQueue::m_fGetHappen(std::vector<Timer *> &happened)
{
    Time time = Time::now();
    std::pair<Time, Timer *> flag = std::pair<Time, Timer *>(time, nullptr);
    std::set<std::pair<Time, Timer *>>::iterator end = m_nTimers.lower_bound(flag);

    std::vector<std::pair<Time, Timer *>> res;
    std::copy(m_nTimers.begin(), end, std::back_inserter(res));
    for (auto t : res)
    {
        happened.push_back(t.second);
    }
    m_nTimers.erase(m_nTimers.begin(), end);
}

void TimerQueue::m_fResetHappened(std::vector<Timer *> &happened)
{
    for (auto t : happened)
    {
        if (t->isRepet())
        {
            t->reset();
            m_fInsertTimer(t); //reinsert
        }
        else
        {
            delete t;
            t = nullptr;
        }
    }
}

void TimerQueue::m_fResetTimeFd(Time expiration)
{
    struct itimerspec newValue;
    bzero(&newValue, sizeof newValue);
    newValue.it_value = howMuchTimeFromNow(expiration);
    int ret = ::timerfd_settime(timerfd, 0, &newValue, &oldValue);
}

TimerQueue::TimerQueue(EventLoop *loop)
{
    //have to create a timer file desp
    m_nFd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    m_pTimeChannel = new Channel(loop, m_nFd);
    m_pTimeChannel->setReadCallback(boost::bind(&TimerQueue::m_fHandleRead, this));
    m_pTimeChannel->enableRead();
    LOG(Debug) << "class TimerQueue constructor\n";
}

void TimerQueue::addTimer(Time &time, timerReadCallback c, bool repet, double interval)
{
    Timer *timer = new Timer(time, c, repet, interval);
    m_fInsertTimer(timer);
}

TimerQueue::~TimerQueue()
{
    //delete all Timer*
    for (auto t : m_nTimers)
        delete t.second;
    close(m_nFd);
    delete m_pTimeChannel;
    m_pTimeChannel = nullptr;
    LOG(Debug) << "class TimerQueue destructor\n";
}