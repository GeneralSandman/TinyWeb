/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*---class MemoryPool---
*STL's allocator is a reference to this class.
*This class is be used to manage memory of each Tcp connection.
*Allocate memory:
*   If the size of memory > 128:alloc memory directly.
*   else the size <= 128:find a memory block in free list.
*Deallocate memory:
*   If the size > 128:deallocate it directly.
*   else :retrive this block to free list.
****************************************
*
*/

#include "memorypool.h"
#include "log.h"

MemoryPool::MemoryPool()
{
    for (int i = 0; i < LIST_SIZE; i++)
        m_nFreeList[i] = nullptr;
    m_nAllocatedSpace = 0;

    LOG(Debug) << "class MemoryPool constructor\n";
}

void *MemoryPool::m_fFillFreeList(size_t s)
{
    //if the heap size > block_num * s
    //  :add to this list block_num nodes
    //else if heap size > s
    //  :add to this list one node
    //else
    //  :invoke m_fGetFromHeap() to get more heap memory
    size_t total_left = (size_t)((char *)m_pHeapEnd - (char *)m_pHeapBegin);
    int block_num = 15;
    if (total_left >= block_num * s)
    {
        //add block_num nodes
        obj **list = m_nFreeList + FREELIST_INDEX(s);
        obj *current_block = nullptr, *next_block = nullptr;

        size_t roundup_size = ROUND_UP(s);
        for (;;)
        {
            current_block = next_block;
            next_block += s;

            if ((char *)m_pHeapEnd - (char *)next_block < s)
            {
                break;
            }

            current_block->p_next = next_block;
        }
    }
    else if (total_left >= s)
    {
        //add one node
    }
    else
    {
        //the heap size is small,add heap memory
        m_fAddMoreHeap(s);
    }

    ///////////new begin//////////////
    obj *result = nullptr;
    int chunk_num = 15;
    char *p_chunk = m_fAllocChunk(s, chunk_num);

    if (1 == chunk_num)
    {
        result = (obj *)p_chunk;
    }
    else if (15 == chunk_num)
    {
        obj **list = m_nFreeList + FREELIST_INDEX(s);

        obj *current_chunk = nullptr,
            *next_chunk = nullptr;
        result = (obj *)p_chunk;

        current_chunk = next_chunk = (obj *)(p_chunk + s);

        for (int i = 1; i < chunk_num - 1; i++)
        {
            current_chunk = next_chunk;
            next_chunk = (obj *)((char *)current_chunk + s);
            current_chunk->p_next = next_chunk;
        }
        current_chunk->p_next = nullptr;
    }
    else
    {
    }

    return (void *)result;

    ////////////
}

char *MemoryPool::m_fAllocChunk(size_t s, int &chunk_num)
{
    char *result = nullptr;

    size_t request_size = s * chunk_num;
    size_t left_size = m_pHeapEnd - m_pHeapBegin;
    if (left_size >= request_size)
    {
        //Heap can provied chunk_num chunks to invoker.
        result = m_pHeapBegin;
        m_pHeapBegin += request_size;
        return result;
    }
    else if (left_size >= s)
    {
        //The number of heap provied is between 1 and chunk_size.
        chunk_num = left_size / s;
        request_size = s * chunk_num;
        result = m_pHeapBegin;
        m_pHeapBegin += request_size;
        return result;
    }
    else
    {
        //Heap even can't provied one chunk.
        //Add more Heap space.

        //reuse the last heap space
        //add it to free list
        size_t malloc_size = 0; //malloc_size%8==0
        if (left_size > 0)
        {
            obj **list = m_nFreeList + FREELIST_INDEX(left_size);
            ((obj *)m_pHeapBegin)->p_next = *list;
            *list = (obj *)m_pHeapBegin;
        }

        m_pHeapBegin = (char *)malloc(malloc_size);
        if (nullptr == m_pHeapBegin)
        {
            //reuse free list
            obj **list_ = nullptr;
            for (int i = s; i < MAXSPACE; i += ALIGN)
            {
                list_ = m_nFreeList + FREELIST_INDEX(i);
                if ((*list_) != nullptr)
                {
                    //FIXME:
                    *list_ = (*list_)->p_next;
                    m_pHeapBegin = (char *)(*list_);
                    m_pHeapEnd = m_pHeapBegin + i;

                    //
                }
            }
        }
    }
}

void MemoryPool::m_fAddMoreHeap(size_t s)
{
    //invoke malloc() to get more heap memory
}

void *MemoryPool::allocate(size_t s)
{
    //if size > MAXSPACE : invoke alloc directly,
    //else :find a free block in free list.
    obj *result = nullptr;
    if (s > MAXSPACE)
    {
        result = (obj *)malloc(s);
        if (result == nullptr)
        {
            //we have handle error
            //set_new_handler
            std::cout << "malloc error\n";
        }
    }
    else
    {
        obj **list = m_nFreeList + FREELIST_INDEX(s);

        result = *list;
        if (result == nullptr)
        {
            //this is a empty list,refill list
            return m_fFillFreeList(ROUND_UP(s));
        }
        *list = result->p_next;
    }
    m_nAllocatedSpace += s;
    return result;
}

void MemoryPool::deallocate(void *p, size_t s)
{
    //if s > MAXSPACE :invoke free() directly,
    //else :add it to free list
    if (s > MAXSPACE)
    {
        free(p);
    }
    else
    {
        obj **list = m_nFreeList + FREELIST_INDEX(s);
        obj *newlist = (obj *)p;
        newlist->p_next = *list;
        *list = newlist;
    }
    m_nAllocatedSpace -= s;
}

void *MemoryPool::reallocate(void *p, size_t oldsize, size_t newsize)
{
    deallocate(p, oldsize);
    return allocate(newsize);
}

MemoryPool::~MemoryPool()
{
    LOG(Debug) << "class MemoryPool destructor\n";
}