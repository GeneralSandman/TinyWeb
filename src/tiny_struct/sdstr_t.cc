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

#include <tiny_base/memorypool.h>
#include <tiny_struct/sdstr_t.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

void sdssetpool(sdstr* str, MemoryPool* pool_)
{
    if (str == nullptr)
        return;

    str->pool = pool_;
}

void sdsnnew(sdstr* str, const char* init, unsigned int len)
{
    if (str == nullptr)
        return;

    str->pool = nullptr;
    str->data = nullptr;
    str->len = 0;
    str->alloc = 0;

    unsigned int alloc;

    if (init == nullptr) {
        alloc = 0;
        len = 0;
    } else
        alloc = ROUND_UP(len);
    // else if ROUND_UP

    void* res = malloc(alloc);
    if (res == nullptr) {
        //FIXME:
        std::cout << "[sds] malloc error\n";
        return;
    }

    // std::cout << "[sds] malloc size(" << alloc << ")\n";

    memset(res, 0, alloc);
    if (nullptr != init && len) {
        memcpy(res, (void*)init, len);
        if (len < alloc)
            *((char*)res + len) = '\0';
    }
    str->len = len;
    str->data = (char*)res;
    str->alloc = alloc;
}

void sdsnew(sdstr* str, const char* init)
{
    unsigned int len = (nullptr == init) ? 0 : strlen(init);
    sdsnnew(str, init, len);
}

void sdsnewempty(sdstr* str, unsigned int alloc)
{
    if (str == nullptr)
        return;

    str->pool = nullptr;
    str->data = nullptr;
    str->len = 0;
    str->alloc = 0;

    alloc = ROUND_UP(alloc);

    void* res = malloc(alloc);
    if (res == nullptr) {
        //FIXME:
        std::cout << "[sds] malloc error\n";
        return;
    }

    // std::cout << "[sds] malloc size(" << alloc << ")\n";
    memset(res, 0, alloc);
    str->data = (char*)res;
    str->alloc = alloc;
}

void sdsnewdup(sdstr* str, const sdstr* src)
{
    if (nullptr == src)
        return;
    sdsnnew(str, src->data, src->len);
}

void sdsMakeSpace(sdstr* str, unsigned int addlen)
{
    unsigned int newlen = addlen + str->len;
    unsigned int avail = str->alloc - str->len;

    if (avail >= addlen)
        return;

    unsigned int newalloc = (0 == str->alloc) ? 8 : str->alloc;

    while (newalloc < newlen) {
        newalloc *= 2;
    }

    void* tmp = realloc((void*)(str->data), newalloc);
    if (nullptr == tmp) {
        std::cout << "[sds] realloc error\n";
        return;
    }

    // std::cout << "[sds] realloc size(" << newalloc << ")\n";

    str->alloc = newalloc;
    str->data = (char*)tmp;
}

void sdsgrowzero(sdstr* str, unsigned int len)
{
    if (nullptr == str)
        return;
    unsigned int old_len = str->len;
    if (old_len >= len) {
        return;
    }

    sdsMakeSpace(str, len - old_len);
    memset(str->data + old_len, 0, (len - old_len));

    str->len = len;
}

void sdsncat(sdstr* dest, const char* src, unsigned int len)
{
    sdsMakeSpace(dest, len);
    //FIXME:handle realloc error

    unsigned oldlen = dest->len;
    unsigned newlen = dest->len + len;
    memcpy((void*)(dest->data + oldlen), src, len);
    *(dest->data + newlen) = '\0';
    dest->len = newlen;
}

void sdscat(sdstr* dest, const char* src)
{
    sdsncat(dest, src, strlen(src));
}

void sdsncatsds(sdstr* dest, const sdstr* src, unsigned int len)
{
    sdsncat(dest, src->data, len);
}

void sdscatsds(sdstr* dest, const sdstr* src)
{
    sdsncat(dest, src->data, src->len);
}

void sdsncpy(sdstr* dest, const char* src, unsigned int len)
{
    unsigned int oldlen = dest->len;
    if (oldlen < len) {
        sdsMakeSpace(dest, len - oldlen);
        if (nullptr == dest)
            return;
    }
    memcpy((void*)(dest->data), src, len);
    *(dest->data + len) = '\0';
    dest->len = len;
}

void sdscpy(sdstr* dest, const char* src)
{
    sdsncpy(dest, src, strlen(src));
}

void sdsncpysds(sdstr* dest, const sdstr* src, unsigned int len)
{
    sdsncpy(dest, src->data, len);
}

void sdscpysds(sdstr* dest, const sdstr* src)
{
    sdsncpy(dest, src->data, src->len);
}

void convertull2str(char* dest, unsigned long long value)
{
    char* p = dest;

    do {
        *p = '0' + (value % 10);
        p++;
        value /= 10;

    } while (value);
    *p = '\0';

    p--;
    char* s = dest;
    char tmp;
    while (s < p) {
        tmp = *s;
        *s = *p;
        *p = tmp;
        s++;
        p--;
    }
}

void convertll2str(char* dest, long long v)
{
    char* p = dest;

    unsigned long long value = (v < 0) ? (-v) : v;
    do {
        *p = '0' + (value % 10);
        p++;
        value /= 10;

    } while (value);
    if (v < 0)
        *p++ = '-';
    *p = '\0';

    p--;
    char* s = dest;
    char tmp;
    while (s < p) {
        tmp = *s;
        *s = *p;
        *p = tmp;
        s++;
        p--;
    }
}

// Only used to init a sds.
void sdssetull(sdstr* dest, unsigned long long value)
{
    char buf[21];
    convertull2str(buf, value);
    sdsnnew(dest, buf, strlen(buf));
}

// Only used to init a sds.
void sdssetll(sdstr* dest, long long value)
{
    char buf[21];
    convertll2str(buf, value);
    sdsnnew(dest, buf, strlen(buf));
}

void sdsclear(sdstr* str)
{
    *(str->data + 0) = '\0';
    str->len = 0;
}

void sdscatvsprintf(sdstr* str, const char* fmt, va_list ap)
{
    va_list copy;
    char staticbuf[1024];
    char* newbuf = staticbuf;
    unsigned int buflen = strlen(fmt) * 2;

    if (buflen > sizeof(staticbuf))
        newbuf = (char*)malloc(buflen);
    else
        buflen = sizeof(staticbuf);

    while (1) {
        newbuf[buflen - 2] = '\0';
        va_copy(copy, ap);
        vsnprintf(newbuf, buflen, fmt, copy);
        va_end(copy);
        if (*(newbuf + buflen - 2) != '\0') {
            buflen *= 2;
            if (newbuf != staticbuf)
                newbuf = (char*)realloc(newbuf, buflen);
            else
                newbuf = (char*)malloc(buflen);
            if (newbuf == nullptr)
                return;
            continue;
        }
        break;
    }

    sdsncat(str, newbuf, strlen(newbuf));

    if (newbuf != staticbuf)
        free((void*)newbuf);
}

void sdscatsprintf(sdstr* str, const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    sdscatvsprintf(str, fmt, ap);
    va_end(ap);
}

void sdsjoinstr(sdstr* dest, const char* src[], unsigned int n, const char* sep, int seplen)
{
    unsigned int newlen = 0;
    for (unsigned int i = 0; i < n; i++) {
        if (nullptr != *(src + i))
            newlen += strlen(*(src + i));
    }
    newlen += (n - 1) * seplen;

    sdsMakeSpace(dest, newlen);

    for (unsigned int i = 0; i < n; i++) {
        if (nullptr != *(src + i))
            sdscat(dest, *(src + i));
        if (i != n - 1)
            sdsncat(dest, sep, seplen);
    }
}

void sdsjoinsds(sdstr* dest, sdstr src[], unsigned int n, const char* sep, int seplen)
{
    unsigned int newlen = 0;
    for (unsigned int i = 0; i < n; i++) {
        if (nullptr != (src + i))
            newlen += (src + i)->len;
    }
    newlen += (n - 1) * seplen;

    sdsMakeSpace(dest, newlen);

    for (unsigned int i = 0; i < n; i++) {
        if (nullptr != (src + i))
            sdscatsds(dest, src + i);
        if (i != n - 1)
            sdsncat(dest, sep, seplen);
    }
}

void printf(sdstr* str)
{
    if (nullptr == str)
        return;
    printf("[sdstr](alloc=%d)(len=%d)(str=\"%.*s\")\n", str->alloc, str->len, str->len, str->data);
}

void sdsdestory(sdstr* str)
{
    free((void*)(str->data));
    str->len = 0;
    str->alloc = 0;
}
