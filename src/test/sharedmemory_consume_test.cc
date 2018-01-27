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

#include "../sharedmemory.h"
#include "../time.h"

#include <iostream>

using namespace std;

int main()
{
    SharedMemory<Time> sharedMemory(10);

    for (int i = 0; i < 10; i++)
    {
        Time tmp = sharedMemory.getData();
        // std::cout << tmp.toFormattedString() << std::endl;
        sleep(1);
    }

    return 0;
}