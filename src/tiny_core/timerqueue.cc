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

#include <tiny_core/timerqueue.h>
#include <tiny_core/timer.h>
#include <tiny_core/channel.h>
#include <tiny_base/log.h>

#include <sys/timerfd.h>
#include <boost/bind.hpp>
#include <unistd.h>
#include <assert.h>
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

unsigned long long TimerQueue::m_nCreatedTimers = 0;

void TimerQueue::m_fHandleRead()
{
    //get happened and invoke the timer's callback
    //checkout those timers if will repet

    std::vector<Timer *> happened;
    m_fGetHappen(happened);

    m_nIsCallingExpiredTimers = true;
    // m_nCancelingTimers.clear();//why
    for (auto t : happened)
        t->run();
    m_nIsCallingExpiredTimers = false;
    m_fResetHappened(happened);
}

void TimerQueue::m_fInsertTimer(Timer *timer)
{
    assert(m_nTimers.size() == m_nActiveTimers.size());

    bool mustResetTimeFd = false;
    Time time = timer->getTime();
    if (m_nTimers.empty())
        mustResetTimeFd = true;
    else if (time < m_nTimers.begin()->first)
        mustResetTimeFd = true;

    //FIXME:handle the return value of insert.
    m_nTimers.insert(std::pair<Time, Timer *>(time, timer));
    m_nActiveTimers.insert(std::pair<Timer *, unsigned long long>(timer, timer->getIdNum()));

    if (mustResetTimeFd)
        m_fResetTimeFd(time);

    assert(m_nTimers.size() == m_nActiveTimers.size());

    //
    //m_fResetTimeFd():If timer which we have added will happen before
    //      make sure this timer can be invoked.
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

    //make sure m_nTimers.size()==m_nActiveTimers.size()
    //first:erase m_nTimers
    m_nTimers.erase(m_nTimers.begin(), end);

    //second:erase m_nActiveTimers
    for (auto t : happened)
    {
        std::pair<Timer *, unsigned long long> removeEntry(t, t->getIdNum());
        m_nActiveTimers.erase(removeEntry);
    }

    assert(m_nTimers.size() == m_nActiveTimers.size());
}

void TimerQueue::m_fResetHappened(std::vector<Timer *> &happened)
{

    for (auto t : happened)
    {
        std::pair<Timer *, unsigned long long>
            timer(t, t->getIdNum());

        if (t->isRepet() &&
                m_nCancelingTimers.find(timer) ==
                m_nCancelingTimers.end())
        {
            //If this timer is a repet timer and
            //haven't been canceled:reset.
            t->reset();
            m_fInsertTimer(t); //reinsert timers which want repet.
        }
        else
        {
            delete t;
            t = nullptr;
        }
    }

    m_nCancelingTimers.clear();
}

void TimerQueue::m_fResetTimeFd(Time expiration)
{
    struct itimerspec newValue;
    bzero(&newValue, sizeof newValue);
    newValue.it_value = howMuchTimeFromNow(expiration);
    ::timerfd_settime(m_nFd, 0, &newValue, NULL);
}

TimerQueue::TimerQueue(EventLoop *loop)
{
    //create a timer file descriptor:no block and close-on-exec
    //create a Channel to handle timer's read event.
    m_pEventLoop = loop;
    m_nFd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    m_pTimeChannel = new Channel(loop, m_nFd);
    m_pTimeChannel->setReadCallback(boost::bind(&TimerQueue::m_fHandleRead, this));
    m_pTimeChannel->enableRead();

    LOG(Debug) << "class TimerQueue constructor\n";
}

TimerId TimerQueue::addTimer(Time &time, timerReadCallback c, bool repet, double interval)
{
    //create timer and add it to queue.
    Timer *timer = new Timer(time, c, repet, interval);
    m_fInsertTimer(timer);
    //FIXME:
    return TimerId(timer, timer->getIdNum());
}

void TimerQueue::cancelTimer(TimerId &timerid)
{
    assert(m_nTimers.size() == m_nActiveTimers.size());

    Time time = timerid.m_pTimer->getTime();
    Timer *timer = timerid.m_pTimer;
    unsigned long long idnum = timerid.m_nIdNum;
    std::pair<Time, Timer *>
        cancelTimer = std::make_pair(time, timer);
    std::pair<Timer *, unsigned long long>
        cancelTimer_ = std::make_pair(timer, idnum);

    // std::set<std::pair<Time, Timer *>>::iterator
        // p = m_nTimers.find(cancelTimer);
    std::set<std::pair<Timer *, unsigned long long>>::iterator
        p_ = m_nActiveTimers.find(cancelTimer_);

    if (p_ != m_nActiveTimers.end())
    {
        m_nTimers.erase(cancelTimer);
        m_nActiveTimers.erase(cancelTimer_);
        delete timerid.m_pTimer;
        timerid.m_pTimer = nullptr;
        //make sure this TimerId is invaild.
    }
    else if (m_nIsCallingExpiredTimers)
    {
        m_nCancelingTimers.insert(cancelTimer_);
        //defer the delect action to
        //m_fResetHappened().
        timerid.m_pTimer = nullptr;
        //make sure this TimerId is invaild.
    }
    else
    {
        //this timerid is expired.
        //ignorce.
    }
    assert(m_nTimers.size() == m_nActiveTimers.size());
}

TimerQueue::~TimerQueue()
{
    //delete all Timers,
    //close timer file descriptor,
    //delete Channel
    for (auto t : m_nTimers)
        delete t.second;
    close(m_nFd);
    delete m_pTimeChannel;
    m_pTimeChannel = nullptr;

    LOG(Debug) << "class TimerQueue destructor\n";
}
