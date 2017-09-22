#include "channel.h"
#include "log.h"

Channel::Channel(EventLoop *loop, int fd)
{
    m_pEventLoop = loop;
    int m_nFd = fd;

    LOG(Debug) << "class Channel constructor\n";
}

void Channel::handleEvent() {}

void Channel::setReadCallback() {}

void Channel::setWriteCallback() {}

void Channel::setCloseCallback() {}

void Channel::setErrorCallback() {}

Channel::~Channel()
{
    LOG(Debug) << "class Channel destructor\n";
}