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


int parseTime(const char *time);

void convertTime2Gmt(const time_t *src, bool isLocal, struct tm *gmt);
void convertTm2Time(const struct tm *gmt, bool isLocal, time_t *time);

void formatHttpTime(const time_t time, sdstr *str);
int deformatHttpTime(const sdstr *str, time_t *time);

void formatCookieTime(const time_t time, sdstr *str);
int deformatCookieTime(const sdstr *str, time_t *time);

#endif // !HTTP_TIME_H
