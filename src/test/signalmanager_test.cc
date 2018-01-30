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

#include <tiny_base/signalmanager.h>
#include <tiny_struct/string_t.h>

#include <signal.h>
#include <iostream>
#include <vector>
using namespace std;

SignalManager man;

vector<Signal> s;

#include <sys/types.h>
#include <unistd.h>

void print(int i)
{
    cout << getpid() << i << endl;
}

SignalCallback c = &print;

#include <string>

void init()
{
    std::string tmp;
    man.addSignal(Signal(SIGTERM, tmp, tmp, c));
}

int main()
{
    init();
    pause();
    return 0;
}
