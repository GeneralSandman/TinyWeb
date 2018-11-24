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

#ifndef HTTP_TIME_H
#define HTTP_TIME_H

#include <tiny_struct/sdstr_t.h>

#include <time.h>

typedef struct formatTime
{
    std::string fmt;
    std::string str;
} formatTime;

int parseTime(const char *time);

void convertTime2Gmt(const time_t *src, bool isLocal, struct tm *gmt);
void convertTm2Time(struct tm *gmt, bool isLocal, time_t *time);

void formate(const struct tm *gmt, formatTime *time);
void formateHttpTime(const time_t time, sdstr *str);
void formateCookieTime(const time_t time, sdstr *str);

void deformate(const formatTime *time, struct tm *gmt);

#endif // !HTTP_TIME_H
