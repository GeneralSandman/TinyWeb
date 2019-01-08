/*
 *Author:GeneralSandman
 *Code:https://github.com/GeneralSandman/TinyWeb
 *E-mail:generalsandman@163.com
 *Web:www.dissigil.cn
 */

/*---class MemoryPool---
 * STL's allocator is a reference to this class.
 * This class is be used to manage memory of each Tcp connection.
 * Allocate memory:
 *   If the size of memory > 128:alloc memory directly.
 *   else the size <= 128:find a memory block in free list.
 * Deallocate memory:
 *   If the size > 128:deallocate it directly.
 *   else :retrive this block to free list.
 ****************************************
 *
 */

#include <tiny_base/log.h>
#include <tiny_base/memorypool.h>

OomHandler BasicAllocator::m_nHandler;

MemoryPool::MemoryPool()
    : m_nAllocatedSpace(0)
    , m_nAllSpace(0)
    , m_pCleanHandlers(nullptr)
    , m_pHeapBegin(nullptr)
    , m_pHeapEnd(nullptr)
    , blocks(nullptr)
    , free_blocks(nullptr)
    , m_nAllocatedLargeSpace(0)
{
    for (int i = 0; i < LIST_SIZE; i++)
        m_nFreeList[i] = nullptr;

    LOG(Debug) << "class MemoryPool constructor\n";
}

void* MemoryPool::m_fFillFreeList(size_t s)
{
    // LOG(Debug) << "fill list\n";
    obj* result = nullptr;

    //We need to test the most effictive chunk_num.
    //chunk_num is a value-result argument,
    //set the chunk_num you want,
    //return the actual chunk_num add to this list.
    int chunk_num = 6;
    char* p_chunk = m_fAllocChunk(s, chunk_num);

    // LOG(Debug) << "alloc chunks(" << chunk_num << "),size(" << s << ")\n";
    if (1 == chunk_num) {
        result = (obj*)p_chunk;
    } else {
        //add chunk_num-1 chunk to free list.
        obj** list = m_nFreeList + FREELIST_INDEX(s);

        obj* current_chunk = nullptr;
        obj* next_chunk = nullptr;
        //return first chunk
        result = (obj*)p_chunk;

        *list = next_chunk = (obj*)(p_chunk + s);

        for (int i = 1; i < chunk_num; i++) {
            //FIXME
            current_chunk = next_chunk;
            next_chunk = (obj*)((char*)next_chunk + s);
            current_chunk->p_next = next_chunk;
        }
        current_chunk->p_next = nullptr;
        // LOG(Debug) << "add chunks(" << chunk_num - 1 << ") to free list\n";
    }

    return (void*)result;
}

char* MemoryPool::m_fAllocChunk(size_t s, int& chunk_num)
{
    char* result = nullptr;
    size_t request_size = s * chunk_num;
    size_t left_size = m_pHeapEnd - m_pHeapBegin;

    if (left_size >= request_size) {
        //Heap has enough space for request.
        // LOG(Debug) << "get space from heap:size(" << request_size << ")\n";
        //Heap can provie chunk_num chunks to free list.
        result = m_pHeapBegin;
        m_pHeapBegin += request_size;
        return result;
    } else if (left_size >= s) {
        //The number of heap provie is between 1 and chunk_num.
        chunk_num = left_size / s;
        request_size = s * chunk_num;
        // LOG(Debug) << "get space from heap:size(" << request_size << ")\n";
        result = m_pHeapBegin;
        m_pHeapBegin += request_size;
        return result;
    } else {
        //Heap even can't provied one chunk.
        //Add more heap space.

        if (left_size > 0) {
            //Reuse the last heap space.
            //Add it to free list.
            obj** list = m_nFreeList + FREELIST_INDEX(left_size);
            ((obj*)m_pHeapBegin)->p_next = *list;
            *list = (obj*)m_pHeapBegin;
            // LOG(Debug) << "add remaining heap-space to list:size(" << left_size
                       // << "),list-index(" << FREELIST_INDEX(left_size) << ")\n";
        }

        //We have to set malloc_size carefully.
        size_t malloc_size = 2 * request_size;
        m_pHeapBegin = (char*)malloc(malloc_size);
        // LOG(Info) << "malloc" << std::endl;
        m_nAllSpace += malloc_size;

        //Update cleanHandlers
        struct cleanup* newCleanHandler = (struct cleanup*)malloc(sizeof(struct cleanup));
        newCleanHandler->data = (void*)m_pHeapBegin;
        newCleanHandler->next = m_pCleanHandlers;
        m_pCleanHandlers = newCleanHandler;

        // if (!newCleanHandler || !m_pHeapBegin)
            // LOG(Debug) << "alloc error---\n";

        if (nullptr == m_pHeapBegin) {
            //Malloc error
            //Reuse free list which size > s
            obj** list_ = nullptr;
            for (int i = s; i < MAXSPACE; i += ALIGN) {
                list_ = m_nFreeList + FREELIST_INDEX(i);
                if ((*list_) != nullptr) {
                    *list_ = (*list_)->p_next;
                    m_pHeapBegin = (char*)(*list_);
                    m_pHeapEnd = m_pHeapBegin + i;
                    return (m_fAllocChunk(s, chunk_num));
                }
            }

            //any free list haven't a free chunk
            //invoke first allocator to retry
            //FIXME:not finished
        }

        //Heap have enough space to free chunk.
        m_pHeapEnd = m_pHeapBegin + malloc_size;
        return (m_fAllocChunk(s, chunk_num));
    }
}

void* MemoryPool::m_fMallocLargeSpace(size_t size)
{
    // LOG(Debug) << "malloc large space:" << size << std::endl;
    void* res = nullptr;
    res = malloc(size);
    if (nullptr == res) {
        LOG(Debug) << "malloc large space error\n";
        return nullptr;
    }

    m_nAllocatedLargeSpace += size;

    // Add to allocated list.
    block_t* new_block = nullptr;
    new_block = (block_t*)allocate(sizeof(block_t));
    new_block->data = res;
    new_block->len = size;
    new_block->next = blocks;
    blocks = new_block;

    return res;
}

void* MemoryPool::allocate(size_t s)
{
    m_nAllocatedSpace += ROUND_UP(s);
    obj* result = nullptr;
    if (s > MAXSPACE) {
        // Get space by BasicAllocator
        // LOG(Debug) << "[MemoryPool allocate] by BasicAllocator\n";
        return BasicAllocator::allocate(s);
    } else {
        // Get free block from free list
        // LOG(Debug) << "[MemoryPool allocate] get space from list:size(" << ROUND_UP(s)
                   // << "),list-index(" << FREELIST_INDEX(s) << ")\n";
        obj** list = m_nFreeList + FREELIST_INDEX(s);
        result = *list;
        if (result == nullptr) {
            return m_fFillFreeList(ROUND_UP(s));
        }
        *list = result->p_next;
    }
    return result;
}

void MemoryPool::deallocate(void* p, size_t s)
{
    //if s > MAXSPACE :invoke free() directly,
    //else :add it to free list
    if (s > MAXSPACE) {
        // LOG(Debug) << "[MemoryPool deallocate] by BasicAllocator\n";
        BasicAllocator::deallocate(p, s);
    } else {
        // LOG(Debug) << "[MemoryPool deallocate] place this space to free list\n";
        obj** list = m_nFreeList + FREELIST_INDEX(s);
        obj* newlist = (obj*)p;
        newlist->p_next = *list;
        *list = newlist;
    }
    m_nAllocatedSpace -= ROUND_UP(s);
}

void* MemoryPool::reallocate(void* p, size_t oldsize, size_t newsize)
{
    // LOG(Debug) << "reallocate\n";
    deallocate(p, oldsize);
    return allocate(newsize);
}

chain_t* MemoryPool::getNewChain(size_t num)
{
    chain_t* new_chain = nullptr;
    chain_t* tmp = nullptr;

    while (num--) {
        new_chain = (chain_t*)allocate(sizeof(chain_t));
        new_chain->buffer = nullptr;

        new_chain->next = tmp;
        tmp = new_chain;
    }

    return new_chain;
}

int MemoryPool::catChain(chain_t* dest,
    chain_t* src,
    unsigned int size)
{
    chain_t* pos;
    chain_t* new_chain;

    for (dest; dest->next != nullptr; dest = dest->next) {
    }

    if (!size)
        size = countChain(src);
    // std::cout << "copy size(" << size << ")" << std::endl;
    new_chain = getNewChain(size);
    if (nullptr == new_chain) {
        std::cout << "get new chain error" << std::endl;
    }

    dest->next = new_chain;
    pos = new_chain;

    while (size--) {
        pos->buffer = src->buffer;

        pos = pos->next;
        src = src->next;
    }

    return 0;
}

void MemoryPool::mallocSpace(chain_t* chain, size_t size)
{
    void* new_mem = nullptr;
    while (nullptr != chain) {
        new_mem = m_fMallocLargeSpace(size);
        chain->buffer = (buffer_t*)allocate(sizeof(buffer_t));
        chain->buffer->begin = (unsigned char*)new_mem;
        chain->buffer->end = (unsigned char*)new_mem + size;
        chain->buffer->used = chain->buffer->begin;
        chain->buffer->deal = chain->buffer->begin;
        chain->buffer->islast = false;
        chain = chain->next;
    }
}

MemoryPool::~MemoryPool()
{
    size_t all = 0;
    for (int i = 0; i < LIST_SIZE; i++) {
        obj* tmp = m_nFreeList[i];
        int size = (i + 1) * 8;
        int num = 0;
        while (tmp != nullptr) {
            num++;
            tmp = tmp->p_next;
        }
        all += num * size;
        if (size) {
            // LOG(Debug) << "size(" << size
                       // << "),nums(" << num << ")\n";
        }
    }

    /*
    LOG(Debug) << "[SmallBlock-Summary]all space size(" << m_nAllSpace
               << "),heap size(" << m_pHeapEnd - m_pHeapBegin
               << "),free list size(" << all
               << "),allocate to user size(" << m_nAllocatedSpace
               << ")\n";
    */
    while (m_pCleanHandlers != nullptr) {
        struct cleanup* cur = m_pCleanHandlers;
        m_pCleanHandlers = m_pCleanHandlers->next;
        free(cur->data);
        free(cur);
    }

    // Free Large block
    size_t num = 0;
    size_t size = 0;
    void* tmp = nullptr;
    while (nullptr != blocks) {
        num++;
        size += blocks->len;

        tmp = (void*)blocks->data;
        blocks = blocks->next;
        free(tmp);
    }
    // LOG(Debug) << "[LargeBlock-Summary]block num(" << num << "),all-size(" << size << ")" << std::endl;

    LOG(Debug) << "class MemoryPool destructor\n";
}
