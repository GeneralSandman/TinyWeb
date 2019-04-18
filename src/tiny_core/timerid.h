/*
 *Author:GeneralSandman
 *Code:https://github.com/GeneralSandman/TinyWeb
 *E-mail:generalsandman@163.com
 *Web:www.dissigil.cn
 */

/*---class TimerId---
 *
 ****************************************
 *
 */

#ifndef TIMER_ID_H
#define TIMER_ID_H

#include <tiny_base/log.h>
#include <tiny_core/timer.h>

class TimerId {
private:
    Timer* m_pTimer;
    unsigned long long m_nIdNum;

public:
    TimerId(Timer* timer = nullptr, unsigned long long idnum = 0)
        : m_pTimer(timer)
        , m_nIdNum(idnum)
    {
        // LOG(Debug) << "class TimerId construct\n";
    }

    TimerId(const TimerId& timerid)
    {
        m_pTimer = timerid.m_pTimer;
        m_nIdNum = timerid.m_nIdNum;
        // LOG(Debug) << "class TimerId construct\n";
    }

    bool isValid() { return m_pTimer != nullptr; }

    friend class Timer;
    friend class TimerQueue;
};

#endif // !TIMER_ID_H
