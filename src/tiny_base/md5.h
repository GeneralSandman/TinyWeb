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

#ifndef TINY_MD5_H
#define TINY_MD5_H

#ifdef HAS_STDINT_H
#include <stdint.h>
#elif defined(HAS_INTTYPES_H)
#include <inttypes.h>
#endif

/* typedef a 32-bit type */
typedef unsigned int UINT4;

/* Data structure for MD5 (Message-Digest) computation */
typedef struct MD5_CTX {
  UINT4 i[2];                   /* number of _bits_ handled mod 2^64 */
  UINT4 buf[4];                                    /* scratch buffer */
  unsigned char in[64];                              /* input buffer */
  unsigned char digest[16];     /* actual digest after MD5Final call */
} MD5_CTX;

void MD5Init (MD5_CTX *mdContext);
void MD5Update (MD5_CTX *, unsigned const char *, unsigned int);
void MD5Final (MD5_CTX *);

void md5(const char* key, int len, char* dst);

#endif
