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

#include "eventloop.h"
#include "epoller.h"
#include "channel.h"
#include "timerqueue.h"
#include "timerid.h"
#include "time.h"
#include "log.h"

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

void EventLoop::cancelTimerId(const TimerId &timerid)
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