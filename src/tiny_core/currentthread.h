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

#ifndef CURRENT_THREAD_H
#define CURRENT_THREAD_H

#include <unistd.h>

namespace CurrentThread
{
extern __thread int t_cachedTid;
extern __thread char t_tidString[32];
extern __thread const char *t_threadName;

void cacheTid();

inline int tid()
{
    if (t_cachedTid == 0)
    {
        cacheTid();
    }
    return t_cachedTid;
}
inline const char *tidString() // for logging
{
    return t_tidString;
}
inline const char *name()
{
    return t_threadName;
}
inline bool isMainThread()
{
    return tid() == getpid();
}
}

#endif // !CURRENT_THREAD_H
