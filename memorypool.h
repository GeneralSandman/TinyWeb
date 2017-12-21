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

#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include <stddef.h>

#define ALIGN 8
#define MAXSPACE 128
#define LIST_SIZE MAXSPACE / ALIGN

//Using union can reduce the expense of managing memory.
union obj {
  union obj *p_next;
  char data[1];
};

inline size_t ROUND_UP(size_t n)
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
  obj *m_nFreeList[LIST_SIZE];

  //mark the boundary of heap
  char *m_pHeapBegin;
  char *m_pHeapEnd;

  void *m_fFillFreeList(size_t);
  char *m_fAllocChunk(size_t, int&);
  void m_fAddMoreHeap(size_t);

public:
  MemoryPool();
  size_t allocatedSpace() { return m_nAllocatedSpace; } //Debug
  void *allocate(size_t);
  void deallocate(void *, size_t);
  void *reallocate(void *, size_t, size_t);
  ~MemoryPool();
};

#endif // !MEMORY_POOL_H
