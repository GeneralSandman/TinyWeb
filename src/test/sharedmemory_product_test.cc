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
#include <deque>

using namespace std;

int main()
{
    // deque<int> q1;
    // std::cout << sizeof(q1) << std::endl;
    // std::cout << q1.size() << std::endl;

    // deque<int> q2;
    // for (int i = 0; i < 10; i++)
    //     q2.push_back(i);
    // std::cout << sizeof(q2) << std::endl;
    // std::cout << q2.size() << std::endl;

    // deque<Time> q3;
    // std::cout << sizeof(q3) << std::endl;
    // std::cout << q3.size() << std::endl;

    // deque<Time> q4;
    // for (int i = 0; i < 10; i++)
    //     q4.push_back(Time::now());
    // std::cout << sizeof(q4) << std::endl;
    // std::cout << q4.size() << std::endl;

    // Shared<Time> a = Shared<Time>(0, 0, 0, 100);
    // std::cout << sizeof(a) << std::endl;

    // Shared<Time> b = Shared<Time>(0, 0, 0, 0);
    // std::cout << sizeof(b) << std::endl;

    SharedMemory<Time> sharedMemory(10);

    for (int i = 0; i < 10; i++)
    {
        std::cout << Time::now().toFormattedString() << std::endl;
        sharedMemory.putData(Time::now());
        sleep(1);
    }

    return 0;
}