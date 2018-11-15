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

#include <tiny_base/memorypool.h>

#include <iostream>

using namespace std;

void test1()
{
    MemoryPool pool;
    for (int i = 0; i < 6;i++)
        pool.allocate(8);
    cout << pool.allocatedSpace() << endl;

}

int main()
{
    test1();
}

int main_()
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
