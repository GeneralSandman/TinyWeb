#include "channel.h"
#include "eventloop.h"
#include "log.h"

void Channel::m_fUpdate()
{
    m_pEventLoop->updateChannel(this);
}

Channel::Channel(EventLoop *loop, int fd)
{
    m_pEventLoop = loop;
    m_nFd = fd;
    m_nEvent = 0;
    m_nREvent = 0;
    m_nFlag = ChannelFlag_New;
    LOG(Debug) << "class Channel constructor\n";
}

void Channel::handleEvent()
{
    if (m_nREvent == EPOLLIN)
    {
        if (m_fReadCallback)
            m_fReadCallback();
    }
}

Channel::~Channel()
{
    LOG(Debug) << "class Channel destructor\n";
}