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

#ifndef SDSTR_H
#define SDSTR_H

#include <tiny_base/memorypool.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string>


typedef struct sdstr
{
    MemoryPool *pool;
    char * data;
    unsigned int len;
    unsigned int alloc;
}sdstr;


void sdsnset(sdstr * str, const char * init, unsigned int len)
{
    if (str == nullptr)
        return;
    str->pool = nullptr;
    if (init == nullptr || len == 0)
    {
        str->data = nullptr;
        str->len = 0;
        str->alloc = 0;
        return;
    }
    void *res = malloc(len);
    if (res == nullptr)
    {
        return;
    }

    memset(res, 0, len);

    str->data = (char *)res;
    str->len = len;
    str->alloc = len;

    memcpy(res, init, len);
}

sdstr* sdsMakeSpace(sdstr *str, unsigned int addlen)
{

    unsigned int newlen = addlen + str->len;
    unsigned int avail = str->alloc - str->len;

    if(avail >= addlen)
        return nullptr;

    void *tmp = realloc((void *)(str->data), newlen);
    if (nullptr == tmp)
    {
        return nullptr;
    }

    str->alloc = newlen;
    str->data = (char *)tmp;
    return str;
}

void sdsncat(sdstr *dest, const char* src, unsigned int len)
{
    sdstr *res = sdsMakeSpace(dest, len);
    if (nullptr == res)
        return;

    unsigned oldlen = res->len;
    unsigned newlen = res->len + len;
    memcpy((void *)(dest->data + oldlen), src, len);
    *(dest->data + newlen) = '\0';
    dest->len = newlen;
}

void sdscat(sdstr *dest, const char* src)
{
    sdsncat(dest, src, strlen(src));
}

void sdsncatsds(sdstr *dest, const sdstr *src, unsigned int len)
{
    sdsncat(dest, src->data, len);
}

void sdscatsds(sdstr *dest, const sdstr *src)
{
    sdsncat(dest, src->data, src->len);
}

void sdsncpy(sdstr *dest, const char* src, unsigned int len)
{
    unsigned int oldlen = dest->len;
    if (oldlen < len)
    {
        sdsMakeSpace(dest, len - oldlen);
        if (nullptr == dest)
            return;
    }
    memcpy((void *)(dest->data), src, len);
    *(dest->data + len) = '\0';
    dest->len = len;
}

void sdscpy(sdstr *dest, const char * src)
{
    sdsncpy(dest, src, strlen(src));
}

void sdsncpysds(sdstr *dest, const sdstr *src, unsigned int len)
{
    sdsncpy(dest, src->data, len);
}

void sdscpysds(sdstr *dest, const sdstr *src)
{
    sdsncpy(dest, src->data, src->len);
}

void convertull2str(char *dest, unsigned long long value)
{
    char *p = dest;

    do
    {
        *p = '0' + (value%10);
        p++;
        value /= 10;

    }while(value);
    *p = '\0';
    
    p--;
    char *s = dest;
    char tmp;
    while(s < p)
    {
        tmp = *s;
        *s = *p;
        *p = tmp;
        s++;
        p--;
    }
}

void convertll2str(char *dest, long long v)
{
    char *p = dest;

    unsigned long long value = (v<0)?(-v):v;
    do
    {
        *p = '0' + (value%10);
        p++;
        value /= 10;

    }while(value);
    if (v < 0)
        *p++ = '-';
    *p = '\0';
    
    p--;
    char *s = dest;
    char tmp;
    while(s < p)
    {
        tmp = *s;
        *s = *p;
        *p = tmp;
        s++;
        p--;
    }

}

// Only used to init a sds.
void sdssetull(sdstr *dest, unsigned long long value)
{
    char buf[21];
    convertull2str(buf, value);
    sdsnset(dest, buf, strlen(buf));
}

// Only used to init a sds.
void sdssetll(sdstr *dest, long long value)
{
    char buf[21];
    convertll2str(buf, value);
    sdsnset(dest, buf, strlen(buf));
}

void sdsclear(sdstr * str)
{
    *(str->data + 0) = '\0';
    str->len = 0;
}

void sdsvsprintf(sdstr *str, const char *fmt, va_list ap)
{
    va_list copy;
    char staticbuf[1024];
    char *newbuf = staticbuf;
    unsigned int buflen = strlen(fmt)*2;

    if (buflen > sizeof(staticbuf))
        newbuf = (char *)malloc(buflen);
    else
        buflen = sizeof(staticbuf);

    while(1)
    {
        newbuf[buflen-2] = '\0';
        va_copy(copy,ap);
        vsnprintf(newbuf, buflen, fmt, copy);
        va_end(copy);
        if (*(newbuf + buflen - 2) != '\0')
        {
            buflen *= 2;
            if (newbuf != staticbuf)
                newbuf = (char *)realloc(newbuf, buflen);
            else
                newbuf = (char *)malloc(buflen);
            if (newbuf == nullptr)
                return;
            continue;
        }
        break;

    }

    sdsnset(str, newbuf, strlen(newbuf));
    
    if (newbuf != staticbuf)
        free(newbuf);
}

void sdssprintf(sdstr *str, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    sdsvsprintf(str, fmt, ap);
    va_end(ap);
}

void printf(sdstr *str)
{
    printf("[sdstr](alloc=%d)(len=%d)(str=\"%.*s\")\n",str->alloc, str->len, str->len, str->data);
}

void destory(sdstr *str)
{
    free((void*)(str->data));
    str->len = 0;
    str->alloc = 0;
}

#endif // !SDSTR_H
