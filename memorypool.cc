/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*---XXX---
*
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

void *MemoryPool::allocate(size_t n)
{
    //if size > MAXSPACE ,invoke alloc directly
    //else find a free block
    obj *result = nullptr;
    if (n > MAXSPACE)
    {
        result = (obj *)malloc(n);
        //we have handle error
    }
    else
    {
        obj **list = m_nFreeList + FREELIST_INDEX(n);

        result = *list;
        if (result == nullptr)
        {
            //refill list
        }
        *list = result->freelist;
    }
    m_nAllocatedSpace += n;
    return result;
}

void MemoryPool::deallocate(void *p, size_t n)
{
    //if n > MAXSPACE ,free this space
    //else add to free list
    if (n > MAXSPACE)
    {
        free(p);
    }
    else
    {
        obj **list = m_nFreeList + FREELIST_INDEX(n);
        obj *newlist = (obj *)p;
        newlist->freelist = *list;
        *list = newlist;
    }
    m_nAllocatedSpace -= n;
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