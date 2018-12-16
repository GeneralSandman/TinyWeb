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
#include <tiny_http/http_model_chunked.h>

int chunked_body(MemoryPool* pool, chunked_context_t* context)
{
    chain_t* in = context->in;
    chain_t** out = &context->out;

    chain_t* tmp = nullptr;

    buffer_t* tmp;

    size_t size = 0;

    while (in) {
        tmp = in->buffer;
        size += tmp->used - tmp.begin;

        in = in->next;
    }

    const char *str = "0000000000000000\r\n";
    if (size) { 
       tmp = pool->getNewChain(1); 
       pool->mallocSpace(tmp, strlen(str));
    } 
}
