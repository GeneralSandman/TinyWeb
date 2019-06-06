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

#include <tiny_base/md5.h>

#include <iostream>
#include <string.h>

static const char b64[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/* "/" replaced with "-" */
static const char b64fss[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-";

int b64cpy(char* dst, const char* src, int n, int fss)
{
    const char* b = fss ? b64fss : b64;
    int i, j;

    j = 0;
    for (i = 0; i < n; i += 3) {
        unsigned char a0, a1, a2;
        a0 = src[i];
        a1 = i < n - 1 ? src[i + 1] : 0;
        a2 = i < n - 2 ? src[i + 2] : 0;
        dst[j++] = b[(a0 >> 2) & 0x3F];
        dst[j++] = b[((a0 << 4) & 0x30) | ((a1 >> 4) & 0x0F)];
        if (i < n - 1)
            dst[j++] = b[((a1 << 2) & 0x3C) | ((a2 >> 6) & 0x03)];
        else
            dst[j++] = '=';
        if (i < n - 2)
            dst[j++] = b[a2 & 0x3F];
        else
            dst[j++] = '=';
    }
    return j;
}

typedef struct md5_test {
    char* url;
} md5_test;

struct md5_test md5_tests[] = {
    {
        .url = "http://www.dissigil.cn/index.html",
    },
    {
        .url = "http://www.dissigil.cn/index.htm",
    },
    {
        .url = "http://dissigil.cn/index.html",
    },
    {
        .url = "http://dissigil.cn/index.htm",
    },
};

void md5_check(char* str)
{
    char res[32];
    memset(res, 0, 32);
    md5(str, strlen(str), res);

    printf("%.*s\n", 32, res);
}

int test1()
{
    unsigned int all = 0;
    unsigned int pass = 0;

    unsigned int len = 4;

    for (unsigned int i = 0; i < len; i++) {
        md5_check(md5_tests[i].url);
    }
}

int main()
{
    test1();
    return 0;
}
