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

#include "sharedmemory.h"
#include "log.h"

#include <sys/mman.h>

void SharedMemory::m_fSync()
{
    //invoke msync
}

SharedMemory::SharedMemory()
{
    //invoke mmap(),
    mmap(NULL, 3 * sizeof(void *), PROT_READ | PROT_WRITE, MAP_SHARED, -1, 0);
    //anonymous memory mapping,needn't create file

    LOG(Debug) << "class SharedMemory constructor\n";
}

void SharedMemory::put()
{
}

void SharedMemory::get()
{
}

SharedMemory::~SharedMemory()
{
    //invoke munmap(),
    LOG(Debug) << "class SharedMemory destructor\n";
}