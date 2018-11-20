/*
 *Author:GeneralSandman
 *Code:https://github.com/GeneralSandman/TinyWeb
 *E-mail:generalsandman@163.com
 *Web:www.dissigil.cn
 */

/*---XXX---
 *
 ****************************************
 *
 */

#include <iostream>

inline int parseTime(const char *time)
{

    return 0;
}

inline void convertTime2Gmt(const time_t *src, bool isLocal, struct tm *gmt)
{
    struct tm *tmp;
    if (isLocal)
        tmp = localtime(src);
    else
        tmp = gmtime(src);
    *gmt = *tmp;
}

inline void convertTm2Time(struct tm * gmt, bool isLocal, time_t *time)
{
    if (!isLocal)
        gmt->tm_hour += 8;
    *time = mktime(gmt);
}

typedef struct formatTime
{
    std::string fmt;
    std::string str;
}formatTime;

inline void formate(const struct tm *gmt, formatTime *time)
{
    char buf[2048];
    int len = strftime(buf, 2048, time->fmt.c_str() , gmt);
    if (len == 0)
    {
        std::cout << "formate time error" << std::endl;
        return ;
    }
    std::string tmp(buf, len);
    time->str = tmp;
}

inline void deformate(const formatTime *time, struct tm * gmt)
{
    strptime(time->str.c_str(), time->fmt.c_str(), gmt);
}




