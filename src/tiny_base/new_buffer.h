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

#ifndef NEW_BUFFER_H
#define NEW_BUFFER_H

#include <tiny_base/memorypool.h> 

#include <string>
#include <unistd.h>

#define K_Buffer_Prepend 8
#define K_Buffer_Size 1024
const char kCRLF[] = "\r\n";

class NewBuffer {
    private:
        chain_t* m_nDatas;
        
    public:
        NewBuffer();

        ~NewBuffer();

};

#endif // !NEW_BUFFER_H
