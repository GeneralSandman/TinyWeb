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

#include "sharedmemory.h"
#include "semaphore.h"
#include "list.h"
#include "log.h"
#include "api.h"

#include <iostream>
#include <deque>
#include <sys/mman.h>

template <typename T>
struct Shared
{
  Semaphore m_nNotEmpty;
  Semaphore m_nNotFull;
  Semaphore m_nMutex;
  std::deque<T> m_nSharedData;

  Shared(int a, int b, int c, int size)
  {
    std::cout << "init" << std::endl;
    m_nNotEmpty = Semaphore(a);
    m_nNotFull = Semaphore(b);
    m_nMutex = Semaphore(c);
    m_nSharedData = std::deque<T>(size);
  }
};

template <typename T>
class SharedMemory
{
private:
  Shared<T> *m_pSharedData;
  void m_fSync(); //FIXME:

public:
  SharedMemory(int size = 100);
  void putData(const T &);
  T getData();
  ~SharedMemory();
};

template <typename T>
void SharedMemory<T>::m_fSync()
{
  //invoke msync
}

template <typename T>
SharedMemory<T>::SharedMemory(int size)
{
  //invoke mmap(),
  m_pSharedData = (Shared<T> *)mmap(NULL, sizeof(Shared<T>),
                                    PROT_READ | PROT_WRITE, MAP_SHARED, -1, 0);
  if (m_pSharedData == MAP_FAILED)
    handle_error("mmap");
  m_pSharedData->m_nNotFull = Semaphore(1);
  m_pSharedData->m_nNotEmpty = Semaphore(0);
  m_pSharedData->m_nMutex = Semaphore(0);
  m_pSharedData->m_nSharedData = std::deque<T>(100);
  //anonymous memory mapping,needn't create file

  LOG(Debug) << "class SharedMemory constructor\n";
}

template <typename T>
void SharedMemory<T>::putData(const T &data)
{
  m_pSharedData->m_nNotFull.wait();
  m_pSharedData->m_nMutex.wait();

  m_pSharedData->m_nSharedData.push_back(data);

  m_pSharedData->m_nMutex.post();
  m_pSharedData->m_nNotEmpty.post(); //enable notEmpty,enable getData
}

template <typename T>
T SharedMemory<T>::getData()
{
  m_pSharedData->m_nNotEmpty.wait();
  m_pSharedData->m_nMutex.wait();

  T tmp;
  tmp = m_pSharedData->m_nSharedData.front();
  m_pSharedData->m_nSharedData.pop_front();

  m_pSharedData->m_nMutex.post();
  m_pSharedData->m_nNotFull.post(); //enable notFull,enable putData

  return tmp;
}

template <typename T>
SharedMemory<T>::~SharedMemory()
{
  //invoke munmap(),
  LOG(Debug) << "class SharedMemory destructor\n";
}

#endif // !SHAREDMEMORY_H
