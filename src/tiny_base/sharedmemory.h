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

#include <TinyWebConfig.h> 
#include <deque>
#include <iostream>
#include <sys/mman.h>

class SharedMemory {
private:
    void* m_pSharedData;
    size_t m_nSize;
    std::string m_nName;

    void* shm_alloc(size_t size);
    void shm_free(void* data, size_t size);

public:
    SharedMemory(size_t size);
    void* getSpace()
    {
        return m_pSharedData;
    }
    ~SharedMemory();
};

#endif // !SHAREDMEMORY_H
