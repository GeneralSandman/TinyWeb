/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*---class MemoryPool---
*MemoryPool is to used to manage memory.
****************************************
*
*/

#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include <tiny_base/log.h>

#include <stddef.h>
#include <boost/function.hpp>

//Information of BasicAllocator

#if 0
#include <new>
#elif !defined(__BASICALLOCATOR_THROW_BAD_ALLOC)
#include <iostream>
#define __BASICALLOCATOR_THROW_BAD_ALLOC     \
  std::cerr << "out of memory" << std::endl; \
  exit(1)
#endif

//out of memory handler
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
        __BASICALLOCATOR_THROW_BAD_ALLOC;
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
        __BASICALLOCATOR_THROW_BAD_ALLOC;
      }
    }
  }

  static OomHandler m_nHandler;

public:
  static void *allocate(size_t size)
  {
    void *result = malloc(size);
    LOG(Debug) << "BasicAllocator malloc " << size << std::endl;
    if (nullptr == result)
      result = m_fOomMalloc(size);
    return result;
  }

  static void *reallocate(void *p, size_t size)
  {
    void *result = realloc(p, size);
    LOG(Debug) << "BasicAllocator realloc " << size << std::endl;
    if (nullptr == result)
      result = m_fOomRealloc(p, size);
    return result;
  }

  static void deallocate(void *p, size_t size)
  {
    free(p);
    LOG(Debug) << "BasicAllocator free " << size << std::endl;
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

inline size_t
ROUND_UP(size_t n)
{
  return (((n) + ALIGN - 1) & ~(ALIGN - 1));
}

inline size_t FREELIST_INDEX(size_t n)
{
  return ((n) + ALIGN - 1) / ALIGN - 1;
}

class MemoryPool
{
private:
  size_t m_nAllocatedSpace; //Debug
  size_t m_nAllSpace;       //Debug
  obj *m_nFreeList[LIST_SIZE];
  struct cleanup *m_pCleanHandlers;

  //Tag the boundary of heap which is the backup memory of free list.
  char *m_pHeapBegin;
  char *m_pHeapEnd;

  void *m_fFillFreeList(size_t);
  char *m_fAllocChunk(size_t, int &);

public:
  MemoryPool();
  size_t allocatedSpace() { return m_nAllocatedSpace; } //Debug
  void *allocate(size_t);
  void deallocate(void *, size_t);
  void *reallocate(void *, size_t, size_t);
  ~MemoryPool();
};

#endif // !MEMORY_POOL_H
