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

#include <tiny_core/currentthread.h>
#include <tiny_base/api.h>

namespace CurrentThread
{

__thread int t_cachedTid = 0;
__thread char t_tidString[32];
__thread const char *t_threadName = "unknown";
void cacheTid()
{
    t_cachedTid = gettid();
    snprintf(t_tidString, sizeof t_tidString, "%5d ",
                     t_cachedTid);
}
}