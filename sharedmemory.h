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

#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H

class Semaphore;

class SharedMemory
{
private:
  Semaphore *m_pEmpty;
  Semaphore *m_pFull;
  void *m_pSharedDate;

  void m_fSync();

public:
  SharedMemory();
  void put();
  void get();
  ~SharedMemory();
};

#endif // !SHAREDMEMORY_H
