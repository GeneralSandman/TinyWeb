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

#include <tiny_core/epoller.h>
#include <tiny_core/channel.h>
#include <tiny_core/time.h>
#include <tiny_base/log.h>
#include <tiny_base/api.h>

#include <sys/epoll.h>
#include <unistd.h>
#include <string.h>

void EPoller::m_fFillActiveChannels(std::vector<Channel *> &active)
{
    for (int i = 0; i < m_nActiveEventNum; i++)
    {
        Channel *channel = (Channel *)m_nActiveEvents[i].data.ptr;
        int revent = m_nActiveEvents[i].events;
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
    m_nActiveEventNum = 0; //no event happened;
    m_nActiveEvents.resize(100);
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
        // int fd = channel->getFd();
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

void EPoller::removeChannel(Channel *channel)
{
    //delete the channel's event struct
    if (channel->isNoneEvent())
    {
        channel->setFlag(ChannelFlag_Deleted);
        m_fUpdate(EPOLL_CTL_DEL, channel);
    }
}

Time EPoller::poll(std::vector<Channel *> &active)
{
    m_nActiveEventNum = epoll_wait(m_nEFd, &(*m_nActiveEvents.begin()),
                                   100, 5000);
    // std::cout << "epoll_wait return:" << m_nActiveEventNum << std::endl;
    Time arriveTime(Time::now());
    if (m_nActiveEventNum > 0)
        m_fFillActiveChannels(active);
    m_nActiveEventNum = 0;
    return arriveTime;
}

EPoller::~EPoller()
{
    close(m_nEFd);
    LOG(Debug) << "class EPoller destructor\n";
}