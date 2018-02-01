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

#include <tiny_base/memorypool.h>

#include <iostream>

using namespace std;

int main()
{
    {
        MemoryPool pool;
        for (int i = 8; i <= 128; i += 8)
            pool.allocate(i);
        cout << pool.allocatedSpace() << endl;
    }
    cout << "-------------------" << endl;
    {

        MemoryPool pool;
        for (int i = 8; i <= 128; i += 8)
            pool.allocate(i);

        for (int i = 8; i <= 128; i += 8)
            pool.allocate(i);
        cout << pool.allocatedSpace() << endl;
    }

    return 0;
}