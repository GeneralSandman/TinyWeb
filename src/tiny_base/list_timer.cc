#include <iostream>
#include "list_timer.h"
namespace timer
{

int ListTimerManager::m_fAddTimer(ListTimer *timer)
{
    //return 0:successful
    //return -1:insert failed
    if (timer == nullptr)
        return -1;
    if (m_pHead == nullptr)
    {
        m_pHead = m_pTail = timer;
        return 0;
    }
    else
    { //find a insert postion
        //the if-else can change to only have else
        //because else can control the m_pHead==nullptr
        ListTimer *tmpHead = new ListTimer(m_pHead, nullptr);
        // m_pHead->m_pPrev = tmpHead;

        ListTimer *tmpprev = tmpHead;
        ListTimer *tmpnext = m_pHead;

        while (tmpnext != nullptr && tmpnext->m_nTime < timer->m_nTime)
        {
            tmpprev = tmpprev->m_pNext;
            tmpnext = tmpnext->m_pNext;
        }

        if (tmpnext == nullptr)
        {
            tmpprev->m_pNext = timer;
            timer->m_pPrev = tmpprev;
            m_pTail = timer;
        }
        else if (tmpnext->m_nTime >= timer->m_nTime)
        {
            tmpprev->m_pNext = timer;
            timer->m_pPrev = tmpprev;

            timer->m_pNext = tmpnext;
            tmpnext->m_pPrev = timer;
        }

        m_pHead = tmpHead->m_pNext;
        m_pHead->m_pPrev = nullptr;
        delete tmpHead;
        return 0;
    }
}

int ListTimerManager::m_fDeleteTimer(ListTimer *timer)
{
    if (timer == nullptr)
        return -1;
    ListTimer *tmpHead = new ListTimer(m_pHead, nullptr);
    ListTimer *tmpTail = new ListTimer(nullptr, m_pTail);

    timer->m_pPrev->m_pNext = timer->m_pNext;
    timer->m_pNext->m_pPrev = timer->m_pPrev;
    delete timer;

    if (tmpHead->m_pNext == tmpTail)
    {
        m_pTail = m_pHead = nullptr;
    }
    else
    {
        m_pHead = tmpHead->m_pNext;
        m_pHead->m_pPrev = nullptr;

        m_pTail = tmpTail->m_pPrev;
        m_pTail->m_pNext = nullptr;
    }

    delete tmpHead;
    delete tmpTail;
    return 0;
}

int ListTimerManager::m_fAdjustTimer(ListTimer *timer)
{
    if (timer == nullptr)
        return -1;

    ListTimer *next = timer->m_pNext;
    if (next == nullptr || next->m_nTime > timer->m_nTime)
    {
        return 0;
    }
    else{
         //delete and insert
    }
    return 0;
}
ListTimerManager::~ListTimerManager()
{
    //delete all node
}

void ListTimerManager::addTimer(ListTimer *timer)
{
    m_fAddTimer(timer);
}

void ListTimerManager::deleteTimer(ListTimer *timer)
{
    m_fDeleteTimer(timer);
}
void ListTimerManager::heatBeat(int seconds)
{
    if (!m_pHead)
        return;
    std::cout << "timer tick\n";
    time_t curr = time(nullptr);

    ListTimer *tmp = m_pHead;
    while (tmp != nullptr && tmp->m_nTime <= curr)
    {
        tmp->m_fTimerHandler(1);
        m_fDeleteTimer(tmp);
        tmp = m_pHead;
    }
}
}