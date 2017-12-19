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

void *MemoryPool::m_fFindFreeBlock(size_t)
{
}

void MemoryPool::m_fFillFreeList(size_t s)
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

            current_block->freelist = next_block;
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
}

void MemoryPool::m_fAddMoreHeap(size_t)
{
    //invoke malloc() to get more heap memory
}

void *MemoryPool::allocate(size_t s)
{
    //if size > MAXSPACE ,invoke alloc directly
    //else find a free block
    obj *result = nullptr;
    if (s > MAXSPACE)
    {
        result = (obj *)malloc(s);
        if (result == nullptr)
        {
            std::cout << "malloc error\n";
        }
        //we have handle error
        //set_new_handler
    }
    else
    {
        obj **list = m_nFreeList + FREELIST_INDEX(s);

        result = *list;
        if (result == nullptr)
        {
            //refill list
            m_fFillFreeList(s);
        }
        *list = result->freelist;
    }
    m_nAllocatedSpace += s;
    return result;
}

void MemoryPool::deallocate(void *p, size_t s)
{
    //if s > MAXSPACE ,free this space
    //else add to free list
    if (s > MAXSPACE)
    {
        free(p);
    }
    else
    {
        obj **list = m_nFreeList + FREELIST_INDEX(s);
        obj *newlist = (obj *)p;
        newlist->freelist = *list;
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