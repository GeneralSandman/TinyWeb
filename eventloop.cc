#include "eventloop.h"
#include "epoller.h"
#include "channel.h"
#include "log.h"

EventLoop::EventLoop()
{
    m_nRunning = true;
    m_pPoller = new EPoller();
    LOG(Debug) << "class EventLoop constructor\n";
}

void EventLoop::loop()
{
    while (m_nRunning)
    {
        m_nActiveChannels.clear();
        m_pPoller->poll(m_nActiveChannels);
        //return active channels

        for (auto t : m_nActiveChannels)
            t->handleEvent();
    }
}

EventLoop::~EventLoop()
{
    LOG(Debug) << "class EventLoop destructor\n";
}