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

#include "semaphore.h"
#include "log.h"

Semphore::Semphore()
{
        LOG(Debug) << "class Semaphore constructor\n";
}

void Semphore::wait()
{
}

void Semphore::post()
{
}

Semphore::~Semphore()
{
        LOG(Debug) << "class Semaphore destructor\n";    
}
