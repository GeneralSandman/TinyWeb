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
#include <tiny_struct/buffer_t.h>
#include <tiny_struct/chain_t.h>

#include <stddef.h>
#include <boost/function.hpp>

//Information of BasicAllocator

#if 0
#include <new>
#elif !defined(TINYWEB_BASICALLOCATOR_THROW_BAD_ALLOC)
#include <iostream>
#define TINYWEB_BASICALLOCATOR_THROW_BAD_ALLOC     \
    std::cerr << "out of memory" << std::endl; \
    exit(1)
#endif

//Out of Memory Handler
typedef boost::function<void()> OomHandler;

class BasicAllocator
{
    private:
        static void *m_fOomMalloc(size_t size)
        {
            LOG(Debug) << std::endl;
            void *result = nullptr;
            OomHandler myHandler;

            while (1)
            {
                myHandler = m_nHandler;
                if (myHandler)
                {
                    myHandler();
                    result = malloc(size);
                    if (nullptr != result)
                        return result;
                }
                else
                {
                    TINYWEB_BASICALLOCATOR_THROW_BAD_ALLOC;
                }
            }
        }

        static void *m_fOomRealloc(void *p, size_t size)
        {

            LOG(Debug) << std::endl;
            void *result = nullptr;
            OomHandler myHandler;

            while (1)
            {
                myHandler = m_nHandler;
                if (myHandler)
                {
                    myHandler();
                    result = realloc(p, size);
                    if (nullptr != result)
                        return result;
                }
                else
                {
                    TINYWEB_BASICALLOCATOR_THROW_BAD_ALLOC;
                }
            }
        }

        static OomHandler m_nHandler;

    public:
        static void *allocate(size_t size)
        {
            void *result = malloc(size);
            LOG(Debug) << "BasicAllocator malloc size(" << size << ")\n";
            if (nullptr == result)
                result = m_fOomMalloc(size);
            return result;
        }

        static void *reallocate(void *p, size_t size)
        {
            void *result = realloc(p, size);
            LOG(Debug) << "BasicAllocator realloc size(" << size << ")\n";
            if (nullptr == result)
                result = m_fOomRealloc(p, size);
            return result;
        }

        static void deallocate(void *p, size_t size)
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
#define LIST_SIZE MAXSPACE / ALIGN

#define ALIGN_LARGE 16

//Using union can reduce the cost of managing memory.
union obj {
    union obj *p_next;
    char data[1];
};

struct cleanup
{
    void *data;
    struct cleanup *next;
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

typedef struct block_t
{
    void * data;
    unsigned long len;
    block_t * next;
}block_t;

class MemoryPool
{
    private:
        size_t m_nAllocatedSpace; //Debug
        size_t m_nAllSpace;       //Debug
        obj *m_nFreeList[LIST_SIZE];
        struct cleanup *m_pCleanHandlers;

        //Tag the boundary of heap which is the backup-memory of free list.
        char *m_pHeapBegin;
        char *m_pHeapEnd;

        //List for large block
        block_t * blocks;
        block_t * free_blocks;
        size_t m_nAllocatedLargeSpace; //Debug

        void *m_fFillFreeList(size_t);
        char *m_fAllocChunk(size_t, int &);

    public:
        MemoryPool();
        size_t allocatedSpace() { return m_nAllocatedSpace; } //Debug
        size_t allocatedLargeSpace() { return m_nAllocatedLargeSpace; }

        void *allocate(size_t);
        void deallocate(void *, size_t);
        void *reallocate(void *, size_t, size_t);

        // Api for large memory
        chain_t* getNewChain(size_t num);
        int catChain(chain_t *dest,
                chain_t *src,
                unsigned int size = 0);
        inline void * mallocLargeSpace(size_t size);
        void mallocSpace(chain_t *chain, size_t size);

        ~MemoryPool();
};

#endif // !MEMORY_POOL_H
