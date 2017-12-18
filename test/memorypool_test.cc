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

#include "../memorypool.h"
#include <iostream>
using namespace std;

int main()
{
    MemoryPool pool;
    int *p = (int *)pool.allocate(4);
    cout << pool.allocatedSpace() << endl;

    return 0;
}