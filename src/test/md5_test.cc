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

static const char b64[65] =
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/* "/" replaced with "-" */
static const char b64fss[65] =
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-";


int
b64cpy(char* dst, const char* src, int n, int fss)
{
    const char *b = fss ? b64fss: b64;
    int i, j;

    j = 0;
    for(i = 0; i < n; i += 3) {
        unsigned char a0, a1, a2;
        a0 = src[i];
        a1 = i < n - 1 ? src[i + 1] : 0;
        a2 = i < n - 2 ? src[i + 2] : 0;
        dst[j++] = b[(a0 >> 2) & 0x3F];
        dst[j++] = b[((a0 << 4) & 0x30) | ((a1 >> 4) & 0x0F)];
        if(i < n - 1)
            dst[j++] = b[((a1 << 2) & 0x3C) | ((a2 >> 6) & 0x03)];
        else
            dst[j++] = '=';
        if(i < n - 2)
            dst[j++] = b[a2 & 0x3F];
        else
            dst[j++] = '=';
    }
    return j;
}

void
md5(unsigned char *key, int len, unsigned char *dst)
{
    static MD5_CTX ctx;
    MD5Init(&ctx);
    MD5Update(&ctx, key, len);
    MD5Final(&ctx);
    memcpy(dst, ctx.digest, 24);
}

int main()
{

    char * str = "123456";
    unsigned char md5buf[18];
    char res[24];
    memset(res, 0, 24);

    md5((unsigned char*)(str), strlen(str), md5buf);
    b64cpy(res, (char*)md5buf, 16, 1);

    std::cout << res << std::endl;

    for (int i=0;i<24;i++) {
        std::cout << i;
        std::cout << " ";
        std::cout << res[i] << std::endl;
    }

    return 0;
}
