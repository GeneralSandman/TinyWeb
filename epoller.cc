#include "epoller.h"
#include "log.h"

#include <sys/epoll.h>

void EPoller::m_fFillActiveChannels(std::vector<Channel *> &)
{
}

EPoller::EPoller()
{
    m_nFd = epoll_create(10000);
    m_nEvents.resize(100);
    LOG(Debug) << "class EPoller constructor\n";
}

void EPoller::poll(std::vector<Channel *> &actitve)
{
    int num = epoll_wait(m_nFd, &(*m_nEvents.begin()),
                         100, 500);
    for (int i = 0; i < num; i++)
    {
        int fd = m_nEvents[i].data.fd;
        int event = m_nEvents[i].events;
        //no finished
    }
}

EPoller::~EPoller()
{
    LOG(Debug) << "class EPoller destructor\n";
}