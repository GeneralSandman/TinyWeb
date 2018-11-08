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

#include <tiny_core/processpool.h>

#include <iostream>

using namespace std;

int main()
{
    {
        ProcessPool pool;
        pool.init();
        pool.createProcess(1);
    }

    return 0;
}
