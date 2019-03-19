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

public:
    HttpModelChunk(MemoryPool* pool)
        : m_pPool(pool)
    {
        // LOG(Debug) << "class HttpModelChunk constructor\n";
    }

    void chunkedChain(chain_t* in, chain_t*& out, bool tail)
    {
        if (tail) {
            chunked_chain_tail(in, out);
        } else {
            chunked_chain(in, out);
        }
    }

    void chunked_chain_tail(chain_t* in,chain_t* & out)
    {
        char tail_str[] = "0\r\n\r\n";
        chain_t* tail_chain = in;
        buffer_t* buffer = nullptr;
        while (tail_chain) {
            buffer = tail_chain->buffer;
            if (buffer->islast)
                break;
            if (nullptr == tail_chain->next)
                break;

            tail_chain = tail_chain->next;
        }

        if (nullptr != buffer) {
            unsigned int empty_size = buffer->end - buffer->used;
            if (empty_size < strlen(tail_str)) {
                chain_t* new_chain = nullptr;
                new_chain = m_pPool->getNewChain(1);
                m_pPool->mallocSpace(new_chain, 1024);
                appendData(new_chain, tail_str, strlen(tail_str));

                m_pPool->catChain(tail_chain, new_chain);
            } else {
                appendData(tail_chain, tail_str, strlen(tail_str));
            }
        }

        out = in;
    }

    void chunked_chain(chain_t* in, chain_t*& out)
    {
        if (nullptr == in)
            return;
        unsigned int all_data_size = countAllDataSize(in);

        sdstr str1;
        char source[] = "0000000000000000\r\n";
        sdsnewempty(&str1, sizeof(source));
        sdscatsprintf(&str1, "%x\r\n", all_data_size);
        printf(&str1);

        chain_t* header = m_pPool->getNewChain(1);
        m_pPool->mallocSpace(header, sizeof(source));
        appendData(header, str1.data, str1.len);

        char tail_str[] = "\r\n";
        chain_t* tail_chain = in;
        buffer_t* buffer = nullptr;
        while (tail_chain) {
            buffer = tail_chain->buffer;
            if (buffer->islast)
                break;
            if (nullptr == tail_chain->next)
                break;

            tail_chain = tail_chain->next;
        }

        if (nullptr != buffer) {
            unsigned int empty_size = buffer->end - buffer->used;
            if (empty_size < strlen(tail_str)) {
                chain_t* new_chain = nullptr;
                new_chain = m_pPool->getNewChain(1);
                m_pPool->mallocSpace(new_chain, 1024);
                appendData(new_chain, tail_str, strlen(tail_str));

                m_pPool->catChain(tail_chain, new_chain);
            } else {
                appendData(tail_chain, tail_str, strlen(tail_str));
            }
        }

        m_pPool->catChain(header, in);
        out = header;

        sdsdestory(&str1);
    }

    ~HttpModelChunk()
    {
        // LOG(Debug) << "class HttpModelChunk destructor\n";
    }
};

#endif
