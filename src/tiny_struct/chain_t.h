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

#ifndef CHAIN_T_H
#define CHAIN_T_H

#include <tiny_struct/buffer_t.h>

typedef struct chain_t
{
    buffer_t *buffer;
    chain_t *next;
}chain_t;

inline unsigned int countChain(chain_t * chain)
{
    unsigned int num = 0;

    while (nullptr != chain)
    {
        num++;
        chain = chain->next;
    }

    return num;
}


#endif
