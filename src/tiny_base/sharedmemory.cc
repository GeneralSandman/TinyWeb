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

#include <tiny_base/sharedmemory.h>
#include <tiny_base/log.h>

SharedMemory::SharedMemory(int size)
    : m_nSize(size)
{
    //invoke mmap(),
    m_pSharedData = mmap(NULL, m_nSize,
                         PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
    if (m_pSharedData == MAP_FAILED)
        handle_error("mmap:");
    //anonymous memory mapping,needn't create file

    LOG(Debug) << "class SharedMemory constructor\n";
}

SharedMemory::~SharedMemory()
{
    //invoke munmap(),
    if (0 != munmap(m_pSharedData, m_nSize))
        handle_error("munmap error:");
    LOG(Debug) << "class SharedMemory destructor\n";
}