#include "currentthread.h"
#include "api.h"
namespace CurrentThread
{
void cacheTid()
{
    t_cachedTid = gettid();
    int n = snprintf(t_tidString, sizeof t_tidString, "%5d ",
                     t_cachedTid);
}
}