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

#ifndef STR_H
#define STR_H

#include <stdio.h>

typedef struct Str
{
    char *data;
    unsigned int len;
} Str;

#define Str(str)                     \
{                                \
    (char *)str, sizeof(str) - 1 \
}

#define NullStr \
{           \
    NULL, 0 \
}

#define setStr(str, text)       \
    (str)->data = (char *)text; \
    (str)->len = sizeof(text) - 1

#define setStrn(str, text, len) \
    (str)->data = (char *)text; \
    (str)->len = len;

#define setStrNull(str) \
    (str)->data = NULL; \
    (str)->len = 0;

inline void printStr(const Str *s)
{
    printf("[Debug Str](len:%d)(%.*s)\n", s->len, s->len, s->data);
};

#endif // !STR_H
