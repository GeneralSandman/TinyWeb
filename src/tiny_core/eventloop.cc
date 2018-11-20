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

#include <tiny_core/eventloop.h>
#include <tiny_core/epoller.h>
#include <tiny_core/channel.h>
#include <tiny_core/timerqueue.h>
#include <tiny_core/timerid.h>
#include <tiny_core/time.h>
#include <tiny_base/log.h>

#include <sys/types.h>
#include <unistd.h>


extern int status_quit_softly;
extern int status_terminate;

EventLoop::EventLoop()
{
    m_nRunning = true;
    m_pPoller = new EPoller();
    m_pTimerQueue = new TimerQueue(this);
    LOG(Debug) << "class EventLoop constructor\n";
}

void EventLoop::updateChannel(Channel *channel)
{
    m_pPoller->updateChannel(channel);
}

void EventLoop::removeChannel(Channel *channel)
{
    m_pPoller->removeChannel(channel);
}

void EventLoop::loop()
{
    while (m_nRunning)
    {
        m_nActiveChannels.clear();
        Time arriveTime = m_pPoller->poll(m_nActiveChannels);
        for (auto t : m_nActiveChannels)
            t->handleEvent(arriveTime);

        //stop this loop if get signal SIGINT SIGTERM SIGKILL SIGQUIT
        if (status_quit_softly == 1 || status_terminate == 1)
        {
            std::cout << "process(" << getpid() << ") quit this eventloop\n";
            m_nRunning = false;
        }
    }
}

TimerId EventLoop::runAt(Time time, timerReadCallback c)
{
    return m_pTimerQueue->addTimer(time, c, false, 0);
}

TimerId EventLoop::runAfter(double second, timerReadCallback c)
{
    Time time = addTime(Time::now(), second);
    return m_pTimerQueue->addTimer(time, c, false, 0);
}

TimerId EventLoop::runEvery(double interval, timerReadCallback c)
{
    Time time = addTime(Time::now(), interval);
    return m_pTimerQueue->addTimer(time, c, true, interval);
}

void EventLoop::cancelTimerId(TimerId &timerid)
{
    m_pTimerQueue->cancelTimer(timerid);
}

EventLoop::~EventLoop()
{
    delete m_pPoller;
    m_pPoller = nullptr;

    delete m_pTimerQueue;
    m_pTimerQueue = nullptr;
    LOG(Debug) << "class EventLoop destructor\n";
}
