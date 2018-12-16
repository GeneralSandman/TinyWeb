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

#ifndef HTTP_MODEL_CHUNKED_H
#define HTTP_MODEL_CHUNKED_H

#include <tiny_base/memorypool.h>

typedef struct chunked_config_t {

    bool enable;

} chunked_config_t;

typedef struct chunked_context_t {

    chain_t* in;
    chain_t* out;

} chunked_config_t;

int chunked_body(MemoryPool* pool, chunked_context_t* context);

#endif
