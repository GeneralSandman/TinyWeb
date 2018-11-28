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

#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H

#include <tiny_base/semaphore.h>
// #include <tiny_base/log.h>
#include <tiny_base/api.h>
#include <tiny_struct/sdstr_t.h>

#include <iostream>
#include <deque>
#include <sys/mman.h>

class SharedMemory
{
private:
  void *m_pSharedData;
  size_t m_nSize;
  sdstr m_nName;

public:
  SharedMemory(int size);
  ~SharedMemory();
};

#endif // !SHAREDMEMORY_H
