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
#include <tiny_struct/chain_t.h>
#include <tiny_struct/buffer_t.h>

#include <iostream>
#include <string>
#include <vector>

using namespace std;

void basicShow()
{
    std::cout << "chain_t size:" << sizeof(chain_t) << std::endl;
    std::cout << "buffer_t size:" << sizeof(buffer_t) << std::endl;
    std::cout << "block_t size:" << sizeof(block_t) << std::endl;

}

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

void test2()
{
    MemoryPool pool;

    chain_t *chain1 = nullptr;
    chain_t *chain2 = nullptr;

    chain1 = pool.getNewChain(8);
    chain2 = pool.getNewChain(8);

    pool.mallocSpace(chain1, 1024);
    pool.mallocSpace(chain2, 1024);

    std::cout << "chain1 size:" << countChain(chain1) << std::endl;
    std::cout << "chain2 size:" << countChain(chain2) << std::endl;


    pool.catChain(chain1, chain2);
    std::cout << "chain1 size:" << countChain(chain1) << std::endl;

    cout << "allocatedSpace:" << pool.allocatedSpace() << endl;
    cout << "allocatedLargeSpace:" << pool.allocatedLargeSpace() << endl;

}

int main()
{
    basicShow();
    // test1();
    test2();
}
