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

#include <tiny_core/timerqueue.h>
#include <tiny_core/timer.h>
#include <tiny_core/time.h>
#include <tiny_base/log.h>

Timer::Timer(Time &time, timerReadCallback c, bool repet, double interval)
{
    m_nTime = time;
    m_nIdNum = TimerQueue::getNextTimerId(); //++
    m_nRepet = repet;
    m_nInterval = interval;
    m_nCallback = c;

    LOG(Debug) << "class Timer constructor\n";
}

void Timer::reset()
{
    if (m_nRepet)
    {
        m_nTime = addTime(Time::now(), m_nInterval);
    }
}

Timer::~Timer()
{
    LOG(Debug) << "class Timer destructor\n";
}