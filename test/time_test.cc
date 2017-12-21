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

#include "../time.h"

#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main()
{
    Time now(Time::now());
    printf("%s\n", now.toString().c_str());
    std::cout << now.toFormattedString() << std::endl;
}
