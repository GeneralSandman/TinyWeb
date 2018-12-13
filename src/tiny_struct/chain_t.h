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

#ifndef HTTP_CHAIN_H
#define HTTP_CHAIN_H

#include <tiny_http/http_buffer.h>

typedef struct http_chain_t
{
    http_buffer_t *buffer;
    http_chain_t *next;
}http_chain_t;



#endif
