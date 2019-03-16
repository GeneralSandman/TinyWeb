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

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#define SDS_MIN_ALLOC_SIZE
typedef struct sdstr {
    MemoryPool* pool;
    char* data;
    unsigned int len;
    unsigned int alloc;
} sdstr;

void sdssetpool(MemoryPool* pool_);

void sdsnnew(sdstr* str, const char* init, unsigned int len);

void sdsnew(sdstr* str, const char* init);

void sdsnewempty(sdstr* str, unsigned int alloc = 0);

void sdsnewdup(sdstr* str, const sdstr* src);

void sdsMakeSpace(sdstr* str, unsigned int addlen);

void sdsgrowzero(sdstr* str, unsigned int len);

void sdsncat(sdstr* dest, const char* src, unsigned int len);

void sdscat(sdstr* dest, const char* src);

void sdsncatsds(sdstr* dest, const sdstr* src, unsigned int len);

void sdscatsds(sdstr* dest, const sdstr* src);

void sdsncpy(sdstr* dest, const char* src, unsigned int len);

void sdscpy(sdstr* dest, const char* src);

void sdsncpysds(sdstr* dest, const sdstr* src, unsigned int len);

void sdscpysds(sdstr* dest, const sdstr* src);

void convertull2str(char* dest, unsigned long long value);

void convertll2str(char* dest, long long v);

// Only used to init a sds.
void sdssetull(sdstr* dest, unsigned long long value);

// Only used to init a sds.
void sdssetll(sdstr* dest, long long value);

void sdsclear(sdstr* str);

void sdscatvsprintf(sdstr* str, const char* fmt, va_list ap);

void sdscatsprintf(sdstr* str, const char* fmt, ...);

void sdsjoinstr(sdstr* dest, const char* src[], unsigned int n, const char* sep, int seplen);

void sdsjoinsds(sdstr* dest, sdstr* src, unsigned int n, const char* sep, int seplen);

void printf(sdstr* str);

void sdsdestory(sdstr* str);

#endif // !SDSTR_H
