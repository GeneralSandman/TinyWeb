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
#include <string>
#include <vector>

using namespace std;

void test1()
{
    MemoryPool pool;
    
    std::vector<void *> alloc16;
    std::vector<void *> alloc32;
    std::vector<void *> alloc48;
    std::vector<void *> alloc56;
    std::vector<void *> alloc88;
    std::vector<void *> alloc128;
    for (int i=0; i<10; i++)
    {
        alloc16.push_back(pool.allocate(16));
        alloc32.push_back(pool.allocate(32));
        alloc48.push_back(pool.allocate(48));
        alloc56.push_back(pool.allocate(56));
        alloc88.push_back(pool.allocate(88));
        alloc128.push_back(pool.allocate(128));
    }
    cout << "allocatedSpace:" << pool.allocatedSpace() << endl;
    for (int i=0; i<10; i++)
    {
        pool.deallocate(alloc16[i], 16);
        pool.deallocate(alloc32[i], 32);
        pool.deallocate(alloc48[i], 48);
        pool.deallocate(alloc56[i], 56);
        pool.deallocate(alloc88[i], 88);
        pool.deallocate(alloc128[i], 128);
    }
    cout << "allocatedSpace:" << pool.allocatedSpace() << endl;
}

int main()
{
    test1();
}
