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

#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H

#include <tiny_core/time.h>
#include <tiny_core/timer.h>

#include <vector>

class EPoller;
class Channel;
class TimerQueue;
class TimerId;

class EventLoop
{
    private:
        bool m_nRunning;
        EPoller *m_pPoller;
        std::vector<Channel *> m_nActiveChannels;
        TimerQueue *m_pTimerQueue;

    public:
        EventLoop();
        void updateChannel(Channel *);
        void removeChannel(Channel *);
        void loop();
        void quit() { m_nRunning = false; }
        TimerId runAt(Time, timerReadCallback);
        TimerId runAfter(double, timerReadCallback);
        TimerId runEvery(double, timerReadCallback);
        void cancelTimerId(TimerId &);

        ~EventLoop();
};

#endif
