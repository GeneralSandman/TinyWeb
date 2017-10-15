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

#include "../buffer.h"

#include <iostream>

using namespace std;

int main()
{
    Buffer out;

    out.append("abc");
    out.append("abc");
    out.append("abc");
    out.append("abc");
    out.append("abc");
    
    cout<<out.get(3)<<endl;
    cout << out.getAll() << endl;

    return 0;
}