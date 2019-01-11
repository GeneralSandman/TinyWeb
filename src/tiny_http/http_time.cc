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

static const char * httpTimeFormat1 = "%a, %d %b %Y %H:%M:%S %Z";
static const char * httpTimeFormat2 = "%A, %d-%b-%y %H:%M:%S %Z";
static const char * httpTimeFormat3 = "%a %b %e %H:%M:%S %Y";


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

void convertTm2Time(const struct tm *gmt, bool isLocal, time_t *time)
{
    struct tm gmt_tmp = *gmt;
    if (!isLocal)
        gmt_tmp.tm_hour += 8;
    *time = mktime(&gmt_tmp);
}


void formatHttpTime(const time_t time, sdstr *str)
{
    struct tm gmt;
    memset((void *)&gmt, 0, sizeof(struct tm));

    convertTime2Gmt(&time, false, &gmt);
    char buf[64];
    int len = strftime(buf, 64, httpTimeFormat1, &gmt);
    sdsncat(str, buf, len);
}

int deformatHttpTime(const sdstr *str, time_t *time)
{
    char* res = nullptr;
    *time = 0;
    struct tm gmt;
    memset((void *)&gmt, 0, sizeof(struct tm));

    res = strptime(str->data, httpTimeFormat1, &gmt);
    if (res == nullptr) {
        std::cout << "strptime error\n";
        return -1;
    }
    convertTm2Time(&gmt, false, time);
    return 0;
}


void formatCookieTime(const time_t time, sdstr *str)
{
    struct tm gmt;
    memset((void *)&gmt, 0, sizeof(struct tm));

    convertTime2Gmt(&time, false, &gmt);
    char buf[64];
    int len = strftime(buf, 64, httpTimeFormat2, &gmt);
    sdsncat(str, buf, len);
}

int deformatCookieTime(const sdstr *str, time_t *time)
{
    char* res = nullptr;
    *time = 0;
    struct tm gmt;
    memset((void *)&gmt, 0, sizeof(struct tm));

    res = strptime(str->data, httpTimeFormat2, &gmt);
    if (res == nullptr) {
        std::cout << "strptime error\n";
        return -1;
    }
    convertTm2Time(&gmt, false, time);
    return 0;
}

