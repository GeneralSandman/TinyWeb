#include "epoller.h"
#include "channel.h"
#include "log.h"
#include "api.h"

#include <sys/epoll.h>
#include <unistd.h>
#include <string.h>

void EPoller::m_fFillActiveChannels(std::vector<Channel *> &active)
{
    for (int i = 0; i < m_nEventNum; i++)
    {
        Channel *channel = (Channel *)m_nEvents[i].data.ptr;
        int revent = m_nEvents[i].events;
        channel->setREvent(revent);
        active.push_back(channel);
    }
}

void EPoller::m_fUpdate(int opt, Channel *channel)
{
    struct epoll_event event;
    bzero(&event, sizeof(event));
    event.events = channel->getEvent();
    event.data.ptr = channel;

    int fd = channel->getFd();
    epoll_ctl(m_nEFd, opt, fd, &event);
    //need handle the error!!!
}

EPoller::EPoller()
{
    m_nEFd = epoll_create(10000);
    m_nEventNum = 0; //no event happened;
    m_nEvents.resize(100);
    LOG(Debug) << "class EPoller constructor\n";
}

void EPoller::updateChannel(Channel *channel)
{
    //add a new channel or modfiy the old one
    int flag = channel->getFlag();
    if (flag == ChannelFlag_New)
    { //add
        int fd = channel->getFd();
        m_nChannels[fd] = channel;
        channel->setFlag(ChannelFlag_Added);
        m_fUpdate(EPOLL_CTL_ADD, channel);
    }
    else if (flag == ChannelFlag_Added)
    { //mod
        int fd = channel->getFd();
        if (!channel->isNoneEvent())
        { //have event to wait;
            m_fUpdate(EPOLL_CTL_MOD, channel);
        }
        else
        {
            //no event to wait;
            //ignore this channel;
            channel->setFlag(ChannelFlag_Deleted);
            m_fUpdate(EPOLL_CTL_DEL, channel);
        }
    }
    else if (flag == ChannelFlag_Deleted)
    { //delete
        if (!channel->isNoneEvent())
        { //have event to wait;
            channel->setFlag(ChannelFlag_Added);
            m_fUpdate(EPOLL_CTL_MOD, channel);
        }
        else
        {
            //still haven't event to wait;
            //do nothing;
        }
    }
}

void EPoller::poll(std::vector<Channel *> &active)
{
    m_nEventNum = epoll_wait(m_nEFd, &(*m_nEvents.begin()),
                             100, 5000);
    // std::cout << "epoll_wait return:" << m_nEventNum << std::endl;
    if (m_nEventNum > 0)
        m_fFillActiveChannels(active);
    m_nEventNum = 0;
}

EPoller::~EPoller()
{
    close(m_nEFd);
    LOG(Debug) << "class EPoller destructor\n";
}