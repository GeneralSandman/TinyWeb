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

class HttpModelChunk {
private:
    MemoryPool* m_pPool;
    bool first;

public:
    HttpModelChunk(MemoryPool* pool)
        : m_pPool(pool)
        , first(true)
    {
        // LOG(Debug) << "class HttpModelChunk constructor\n";
    }

    void* chunked_begin(chain_t* in)
    {
        if (nullptr == in)
            return nullptr;

        char source_first[] = "0000000000000000\r\n";
        char source_middle[] = "\r\n0000000000000000\r\n";
        char source_end[] = "\r\n";

        // TODO:
        if (first) {
            appendData(in, source_first, strlen(source_first));
        } else {
            appendData(in, source_middle, strlen(source_middle));
        }

        return in->buffer->begin;
    }

    void chunked_end(chain_t* in, void* chunk_len_begin, unsigned int chunk_len = 0)
    {
        if (nullptr == in)
            return;

        char source_first[] = "0000000000000000\r\n";
        char source_middle[] = "\r\n0000000000000000\r\n";
        char source_end[] = "\r\n";
        sdstr str1;

        if (first) {
            sdsnewempty(&str1, sizeof(source_first));
            sdscatsprintf(&str1, "%016x\r\n", chunk_len);
            first = false;

        } else {

            sdsnewempty(&str1, sizeof(source_middle));
            sdscatsprintf(&str1, "\r\n%016x\r\n", chunk_len);
        }

        printf(&str1);
        memcpy(chunk_len_begin, (const void*)str1.data, str1.len);

        if (chunk_len == 0) {
            // LOG(Debug) << "append end \r\n"
                    //    << std::endl;
            appendData(in, source_end, strlen(source_end));
        }

        sdsdestory(&str1);
    }

    ~HttpModelChunk()
    {
        // LOG(Debug) << "class HttpModelChunk destructor\n";
    }
};

#endif
