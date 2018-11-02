/*
 *Author:GeneralSandman
 *Code:https://github.com/GeneralSandman/TinyWeb
 *E-mail:generalsandman@163.com
 *Web:www.dissigil.cn
 */

/*---siample dynamic string---
 *
 ****************************************
 *
 */

#ifndef SDSTR_H
#define SDSTR_H

#include <stdio.h>

typedef struct Str
{
    char buf[];
    unsigned int len;
    unsigned int fre;
} Str;

inline void printStr(const Str *s)
{
    printf("[Debug Str](len:%d,free:%d)(%.*s)\n", s->len, s->fre, s->len, s->buf);
};

#endif // !SDSTR_H
