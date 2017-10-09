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

SharedMemory::SharedMemory()
{
    LOG(Debug) << "class SharedMemory constructor\n";
}

SharedMemory::~SharedMemory()
{
    LOG(Debug) << "class SharedMemory destructor\n";
}