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
#include <tiny_struct/sdstr_t.h>

typedef struct chunked_t {
    MemoryPool* pool;
    unsigned int all_data_size;
    chain_t* out;
};

void chunked_init(chunked_t* chunk, MemoryPool* pool_)
{
    chunk->pool = pool_;
    chunk->all_data_size = 0;
    chunk->out = nullptr;
}


chain_t* chunked_chain(chunked_t* chunk, chain_t* out)
{
    chunk->all_data_size = countAllDataSize(out);
    chunk->out = out;

    char source[] = "0000000000000000\r\n";

    sdstr str1;
    sdsnewempty(&str1, sizeof(source));

    std::cout << "all data size:" << chunk->all_data_size << std::endl;
    sdscatsprintf(&str1, "%x\r\n", chunk->all_data_size);
    printf(&str1);

    chain_t* header = chunk->pool->getNewChain(1);
    chunk->pool->mallocSpace(header, sizeof(source));
    appendData(header, str1.data, str1.len);
    appendData(out, "\r\n", 2);

    chunk->pool->catChain(header, out);



    sdsdestory(&str1);
    return header;
}


#endif
