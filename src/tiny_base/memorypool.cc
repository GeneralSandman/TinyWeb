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

#include <tiny_base/memorypool.h>
#include <tiny_base/log.h>

OomHandler BasicAllocator::m_nHandler;

MemoryPool::MemoryPool()
    : m_nAllocatedSpace(0),
    m_nAllSpace(0),
    m_pHeapBegin(nullptr),
    m_pHeapEnd(nullptr),
    m_pCleanHandlers(nullptr)
{
    for (int i = 0; i < LIST_SIZE; i++)
        m_nFreeList[i] = nullptr;

    LOG(Debug) << "class MemoryPool constructor\n";
}

void *MemoryPool::m_fFillFreeList(size_t s)
{
    LOG(Debug) << "fill list\n";
    obj *result = nullptr;

    //We need to test the most effictive chunk_num.
    //chunk_num is a value-result argument,
    //set the chunk_num you want,
    //return the actual chunk_num add to this list.
    int chunk_num = 6;
    char *p_chunk = m_fAllocChunk(s, chunk_num);

    if (1 == chunk_num)
    {
        LOG(Debug) << "alloc chunk number is 1\n";
        result = (obj *)p_chunk;
    }
    else //chunk_num >= 2
    {
        LOG(Debug) << "alloc chunks(" << chunk_num << "),size(" << s << ")\n";
        //add chunk_num-1 chunk to free list.
        obj **list = m_nFreeList + FREELIST_INDEX(s);

        obj *current_chunk = nullptr,
            *next_chunk = nullptr;
        //return first chunk
        result = (obj *)p_chunk;

        *list = next_chunk = (obj *)(p_chunk + s);

        for (int i = 1; i < chunk_num; i++)
        {
            //FIXME
            current_chunk = next_chunk;
            next_chunk = (obj *)((char *)next_chunk + s);
            current_chunk->p_next = next_chunk;
        }
        current_chunk->p_next = nullptr;
        LOG(Debug) << "add chunks(" << chunk_num - 1 << ") to free list\n";
    }

    return (void *)result;
}

char *MemoryPool::m_fAllocChunk(size_t s, int &chunk_num)
{
    //alloc chunk from self heap or alloc()
    char *result = nullptr;
    size_t request_size = s * chunk_num;
    size_t left_size = m_pHeapEnd - m_pHeapBegin;

    if (left_size >= request_size)
    {
        //Heap has enough space for request.
        LOG(Debug) << "get space from heap:size(" << request_size << ")\n";
        //Heap can provie chunk_num chunks to free list.
        result = m_pHeapBegin;
        m_pHeapBegin += request_size;
        return result;
    }
    else if (left_size >= s)
    {
        //The number of heap provie is between 1 and chunk_num.
        chunk_num = left_size / s;
        request_size = s * chunk_num;
        LOG(Debug) << "get space from heap:size(" << request_size << ")\n";
        result = m_pHeapBegin;
        m_pHeapBegin += request_size;
        return result;
    }
    else
    {
        //Heap even can't provied one chunk.
        //Add more heap space.

        if (left_size > 0)
        {
            //Reuse the last heap space
            //Add it to free list
            obj **list = m_nFreeList + FREELIST_INDEX(left_size);
            ((obj *)m_pHeapBegin)->p_next = *list;
            *list = (obj *)m_pHeapBegin;
            LOG(Debug) << "add remaining heap-space to list:size(" << left_size 
                << "),list-index(" << FREELIST_INDEX(left_size) << ")\n";
        }

        //We have to set malloc_size carefully.
        size_t malloc_size = 2 * request_size; //malloc_size%8==0
        m_pHeapBegin = (char *)malloc(malloc_size);
        m_nAllSpace += malloc_size;

        //Update cleanHandlers
        struct cleanup *newCleanHandler =
            (struct cleanup *)malloc(sizeof(struct cleanup));
        newCleanHandler->data = (void *)m_pHeapBegin;
        newCleanHandler->next = m_pCleanHandlers;
        m_pCleanHandlers = newCleanHandler;

        LOG(Debug) << "malloc memory directly by system call\n";
        if (nullptr == m_pHeapBegin)
        {
            //Malloc error
            //Reuse free list which size > s
            obj **list_ = nullptr;
            for (int i = s; i < MAXSPACE; i += ALIGN)
            {
                list_ = m_nFreeList + FREELIST_INDEX(i);
                if ((*list_) != nullptr)
                {
                    *list_ = (*list_)->p_next;
                    m_pHeapBegin = (char *)(*list_);
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

void *MemoryPool::allocate(size_t s)
{
    m_nAllocatedSpace += ROUND_UP(s);
    //if size > MAXSPACE : invoke BasicAllocator::allocate,
    //else :find a free block in free list.
    obj *result = nullptr;
    if (s > MAXSPACE)
    {
        LOG(Debug) << "MemoeyPool allocate by BasicAllocator\n";
        return BasicAllocator::allocate(s);
    }
    else
    {
        LOG(Debug) << "MemoeyPool get space from list:size(" << ROUND_UP(s)
                    << "),list-index(" << FREELIST_INDEX(s) << ")\n";
        obj **list = m_nFreeList + FREELIST_INDEX(s);
        result = *list;
        if (result == nullptr)
        {
            //This is a empty list,fill list
            return m_fFillFreeList(ROUND_UP(s));
        }
        *list = result->p_next;
    }
    return result;
}

void MemoryPool::deallocate(void *p, size_t s)
{
    //if s > MAXSPACE :invoke free() directly,
    //else :add it to free list
    if (s > MAXSPACE)
    {
        LOG(Debug) << "MemoeyPool deallocate by BasicAllocator\n";
        BasicAllocator::deallocate(p, s);
    }
    else
    {
        LOG(Debug) << "MemoeyPool place this space to free list\n";
        obj **list = m_nFreeList + FREELIST_INDEX(s);
        obj *newlist = (obj *)p;
        newlist->p_next = *list;
        *list = newlist;
    }
    m_nAllocatedSpace -= ROUND_UP(s);
}

void *MemoryPool::reallocate(void *p, size_t oldsize, size_t newsize)
{
    LOG(Debug) << "reallocate\n";
    deallocate(p, oldsize);
    return allocate(newsize);
}

MemoryPool::~MemoryPool()
{
    size_t all = 0;
    for (int i = 0; i < LIST_SIZE; i++)
    {
        obj *tmp = m_nFreeList[i];
        int size = (i + 1) * 8;
        int num = 0;
        while (tmp != nullptr)
        {
            num++;
            tmp = tmp->p_next;
        }
        all += num * size;
        LOG(Debug) << "size(" << size
            << "),nums(" << num << ")\n";
    }

    LOG(Debug) << "[Summary]all space size(" << m_nAllSpace
        << "),heap size(" << m_pHeapEnd - m_pHeapBegin
        << "),free list size(" << all
        << "),allocate to user size(" << m_nAllocatedSpace
        << ")\n";
    while (m_pCleanHandlers != nullptr)
    {
        struct cleanup *cur = m_pCleanHandlers;
        m_pCleanHandlers = cur->next;
        free(cur->data);
        free(cur);
        LOG(Debug) << "free\n";
    }

    LOG(Debug) << "class MemoryPool destructor\n";
}
