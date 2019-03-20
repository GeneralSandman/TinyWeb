/*
 *Author:GeneralSandman
 *Code:https://github.com/GeneralSandman/TinyWeb
 *E-mail:generalsandman@163.com
 *Web:www.dissigil.cn
 */

/*---class MemoryPool---
 *  MemoryPool is to used to manage memory.
 ****************************************
 *
 */

#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include <tiny_base/log.h>

#include <boost/function.hpp>
#include <stddef.h>
#include <math.h>

//Information of BasicAllocator

#if 0
#include <new>
#elif !defined(TINYWEB_BASICALLOCATOR_THROW_BAD_ALLOC)
#include <iostream>
#define TINYWEB_BASICALLOCATOR_THROW_BAD_ALLOC \
    std::cerr << "out of memory" << std::endl; \
    exit(1)
#endif

//Out of Memory Handler
typedef boost::function<void()> OomHandler;

class BasicAllocator {
private:
    static void* m_fOomMalloc(size_t size)
    {
        LOG(Debug) << std::endl;
        void* result = nullptr;
        OomHandler myHandler;

        while (1) {
            myHandler = m_nHandler;
            if (myHandler) {
                myHandler();
                result = malloc(size);
                if (nullptr != result)
                    return result;
            } else {
                TINYWEB_BASICALLOCATOR_THROW_BAD_ALLOC;
            }
        }
    }

    static void* m_fOomRealloc(void* p, size_t size)
    {

        LOG(Debug) << std::endl;
        void* result = nullptr;
        OomHandler myHandler;

        while (1) {
            myHandler = m_nHandler;
            if (myHandler) {
                myHandler();
                result = realloc(p, size);
                if (nullptr != result)
                    return result;
            } else {
                TINYWEB_BASICALLOCATOR_THROW_BAD_ALLOC;
            }
        }
    }

    static OomHandler m_nHandler;

public:
    static void* allocate(size_t size)
    {
        void* result = malloc(size);
        LOG(Debug) << "BasicAllocator malloc size(" << size << ")\n";
        if (nullptr == result)
            result = m_fOomMalloc(size);
        return result;
    }

    static void* reallocate(void* p, size_t size)
    {
        void* result = realloc(p, size);
        LOG(Debug) << "BasicAllocator realloc size(" << size << ")\n";
        if (nullptr == result)
            result = m_fOomRealloc(p, size);
        return result;
    }

    static void deallocate(void* p, size_t size)
    {
        free(p);
        LOG(Debug) << "BasicAllocator free size(" << size << ")\n";
    }

    static OomHandler setHandler(OomHandler h)
    {
        OomHandler tmp = m_nHandler;
        m_nHandler = h;
        return tmp;
    }
};

//Information of MemoryPool

#define ALIGN 8
#define MAXSPACE 128
#define LIST_SIZE (MAXSPACE / ALIGN)

#define ALIGN_LARGE 16

//Using union can reduce the cost of managing memory.
union obj {
    union obj* p_next;
    char data[1];
};

struct cleanup {
    void* data;
    struct cleanup* next;
};

inline size_t ROUND_UP(size_t n)
{
    return (((n) + ALIGN - 1) & ~(ALIGN - 1));
}

inline size_t ROUND_UP16(size_t n)
{
    return (((n) + ALIGN_LARGE - 1) & ~(ALIGN_LARGE - 1));
}

inline size_t FREELIST_INDEX(size_t n)
{
    return ((n) + ALIGN - 1) / ALIGN - 1;
}

// inline size_t logarithm2(size_t size)
// {
//     float res = log2(size);
//     size_t res_int = (size_t)(res);

//     return (res - res_int) > 0 ? res_int + 1 : res_int;
// }

inline size_t ROUND_UP2N(size_t size)
{
    float res = log2(size);
    size_t res_int = (size_t)(res);

    res = (res - res_int) > 0 ? res_int + 1 : res_int;

    return pow(2, res);
}

inline size_t BLOCK_FREELIST_INDEX(size_t size)
{
    float res = log2(size);
    size_t res_int = (size_t)(res);

    res = (res - res_int) > 0 ? res_int + 1 : res_int;

    return res - 8;
}

struct block_t;

typedef struct buffer_t {
    unsigned char* begin;
    unsigned char* end;

    unsigned char* used;
    unsigned char* deal;

    bool islast;

    // used for file.
    bool sendfile;
    bool valid;
    int file_fd;
    off_t offset;
    unsigned int len;

    // only used for memorypool.
    block_t* block;

} buffer_t;

typedef struct chain_t {
    buffer_t* buffer;
    chain_t* next;
} chain_t;

typedef struct block_t {
    void* data;
    unsigned long len;
    block_t* next;
} block_t;

inline void buffer_init(buffer_t* buffer)
{
    if (nullptr == buffer)
        return;
    buffer->begin = nullptr;
    buffer->end = nullptr;
    buffer->used = nullptr;
    buffer->deal = nullptr;
    buffer->islast = false;
}

inline void block_init(block_t* block)
{
    if (nullptr == block)
        return;
    block->data = nullptr;
    block->len = 0;
    block->next = nullptr;
}

inline unsigned int countChain(chain_t* chain)
{
    unsigned int num = 0;

    while (nullptr != chain) {
        num++;
        chain = chain->next;
    }

    return num;
}

inline void clearData(chain_t* chain)
{
    buffer_t* buffer = nullptr;
    while (nullptr != chain) {
        buffer = chain->buffer;
        buffer->used = buffer->begin;
        buffer->deal = buffer->begin;
        buffer->islast = false;
        chain = chain->next;
    }
}

inline unsigned int countAllBufferSize(const chain_t* chain)
{
    unsigned int all_buffer_size = 0;
    const chain_t* tmp = chain;
    buffer_t* buffer;
    while (tmp != nullptr) {
        buffer = tmp->buffer;
        all_buffer_size += (buffer->end - buffer->begin);
        tmp = tmp->next;
    }
    return all_buffer_size;
}

inline unsigned int countAllDataSize(const chain_t* chain)
{
    unsigned int all_buffer_size = 0;
    const chain_t* tmp = chain;
    buffer_t* buffer;
    while (tmp != nullptr) {
        buffer = tmp->buffer;
        all_buffer_size += (buffer->used - buffer->begin);
        tmp = tmp->next;
    }
    return all_buffer_size;
}

inline unsigned int countAllNoDealSize(const chain_t* chain)
{
    unsigned int all_nodeal_size = 0;
    const chain_t* tmp = chain;
    buffer_t* buffer;
    while (tmp != nullptr) {
        buffer = tmp->buffer;
        all_nodeal_size += (buffer->used - buffer->deal);
        tmp = tmp->next;
    }
    return all_nodeal_size;
}

inline chain_t* appendData(chain_t* dest, const char* data, unsigned int len)
{
    // Have same code with appendData() in file.h

    // Make sure have enough space.
    if (nullptr == dest || nullptr == data || 0 == len) {
        return dest;
    }

    chain_t* chain;
    buffer_t* buffer;
    unsigned int buff_size;
    unsigned int predata_size;
    unsigned int empty_size;

    const char* pos = data;
    unsigned int left = len;
    unsigned int to_write = 0;

    // LOG(Debug) << "append size:" << left << std::endl;

    chain = dest;
    while (left && nullptr != chain) {
        buffer = chain->buffer;
        buff_size = buffer->end - buffer->begin;
        predata_size = buffer->used - buffer->begin;
        empty_size = buff_size - predata_size;

        if (!empty_size) {
            // This chain is full, change to next chain.
            buffer->islast = false;
            chain = chain->next;
            continue;
        }

        to_write = (left > empty_size) ? empty_size : left;
        memcpy((void*)buffer->used, (const void*)pos, to_write);
        // LOG(Debug) << "buffer:all-size(" << buff_size
        // << "),predata-size(" << predata_size
        // << "),postdata-size(" << predata_size + to_write << ")\n";

        buffer->used = buffer->used + to_write;
        buffer->islast = true;

        left -= to_write;
        pos += to_write;

        if (left) {
            // This chain is full, change to next chain.
            buffer->islast = false;
            chain = chain->next;
        }
    }

    if (nullptr == chain) {
        return chain;
    }

    empty_size = chain->buffer->end - chain->buffer->used;
    if (empty_size) {
        chain->buffer->islast = true;
        return chain;
    } else {
        // empty_size
        chain->buffer->islast = false;
        if (nullptr != chain->next) {
            chain->next->buffer->islast = true;
            return chain->next;
        }
        return chain->next;
    }
}

class MemoryPool {
private:
    size_t m_nAllocatedSpace;
    size_t m_nAllSpace;
    obj* m_nFreeList[LIST_SIZE];
    struct cleanup* m_pCleanHandlers;

    //Tag the boundary of heap which is the backup-memory of free list.
    char* m_pHeapBegin;
    char* m_pHeapEnd;

    //List for large block
    block_t* blocks;
    block_t* free_blocks;
    size_t m_nAllocatedLargeSpace;

    void* m_fFillFreeList(size_t);
    char* m_fAllocChunk(size_t, int&);

    block_t* m_fMallocLargeSpace(size_t size);
    void m_fFreeLargeSpace(block_t*);

public:
    MemoryPool();

    size_t allocatedSpace() { return m_nAllocatedSpace; } //Debug
    size_t allocatedLargeSpace() { return m_nAllocatedLargeSpace; }

    void* allocate(size_t);
    void deallocate(void*, size_t);
    void* reallocate(void*, size_t, size_t);

    // Api for large memory
    chain_t* getNewChain(size_t num);
    int catChain(chain_t* dest,
        chain_t* src,
        unsigned int size = 0);
    bool mallocSpace(chain_t* chain, size_t size);

    bool truncateChain(chain_t* chain, unsigned int size, unsigned int buffer_size = 0);

    // chain_t* appendData(const chain_t* dest, const sdstr* str)
    // {
    //     if (nullptr == dest || nullptr == str)
    //         return dest;
    //     // O(N)
    //     // TODO: update chain_t & buffer_t.
    //     chain_t* tmp_chain = nullptr;
    //     buffer_t* buffer = nullptr;

    //     tmp_chain = dest;
    //     while(nullptr != tmp_chain) {
    //         buffer = tmp_chain->buffer;

    //         if ()
    //     }

    //     if (nullptr == tmp_chain || buffer->end - buffer->used < str->len) {
    //         // add new chain.
    //     } else {

    //     }
    // }

    ~MemoryPool();
};

#endif // !MEMORY_POOL_H
