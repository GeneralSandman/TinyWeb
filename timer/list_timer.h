#ifndef LISTTIMER_H
#define LISTTIMER_H
#include <time.h>
namespace timer
{

class ListTimer
{
  public:
    time_t m_nTime;
    void (*m_fTimerHandler)(int);

    ListTimer *m_pNext;
    ListTimer *m_pPrev;

  public:
    ListTimer(ListTimer *n = nullptr,
              ListTimer *p = nullptr) : m_pNext(n),
                                        m_pPrev(p) {}
    ~ListTimer() {}
};
class ListTimerManager
{

  private:
    ListTimer *m_pHead;
    ListTimer *m_pTail;
    int m_fAddTimer(ListTimer *);
    int m_fDeleteTimer(ListTimer *);
    int m_fAdjustTimer(ListTimer *);

  public:
    ListTimerManager() : m_pHead(nullptr),
                         m_pTail(nullptr) {}
    ~ListTimerManager();

    void addTimer(ListTimer *);
    void deleteTimer(ListTimer *);
    void heatBeat(int);
};
}

#endif