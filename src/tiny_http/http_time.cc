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

#include <tiny_http/http_time.h>
#include <tiny_struct/sdstr_t.h>

#include <time.h>


int parseTime(const char *time)
{
    return 0;
}

void convertTime2Gmt(const time_t *src, bool isLocal, struct tm *gmt)
{
    struct tm *tmp;
    if (isLocal)
        tmp = localtime(src);
    else
        tmp = gmtime(src);
    *gmt = *tmp;
}

void convertTm2Time(struct tm *gmt, bool isLocal, time_t *time)
{
    if (!isLocal)
        gmt->tm_hour += 8;
    *time = mktime(gmt);
}

void formate(const struct tm *gmt, formatTime *time)
{
    char buf[128];
    int len = strftime(buf, 128, time->fmt.c_str(), gmt);
    std::string tmp(buf, len);
    time->str = tmp;
}

void formateHttpTime(const time_t time, sdstr *str)
{
    struct tm gmt;
    memset((void *)&gmt, 0, sizeof(struct tm));

    convertTime2Gmt(&time, false, &gmt);
    char buf[64];
    int len = strftime(buf, 64, "%a, %d %b %Y %H:%M:%S %Z", &gmt);
    sdsncat(str, buf, len);
}

void formateCookieTime(const time_t time, sdstr *str)
{
    struct tm gmt;
    memset((void *)&gmt, 0, sizeof(struct tm));

    convertTime2Gmt(&time, false, &gmt);
    char buf[64];
    int len = strftime(buf, 64, "%A, %d-%b-%y %H:%M:%S %Z", &gmt);
    sdsncat(str, buf, len);
}

void deformate(const formatTime *time, struct tm *gmt)
{
    strptime(time->str.c_str(), time->fmt.c_str(), gmt);
}

