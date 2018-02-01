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

#include <tiny_core/worker.h>
#include <tiny_core/process.h>

#include <iostream>
#include <unistd.h>
using namespace std;

int main()
{
    Worker worker1(1, "worker1");
    Process *newProc = new Process("process1", 1, &worker1);

    sleep(5);
    delete newProc;
    return 0;
}