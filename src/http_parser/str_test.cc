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

#include "str_t.h"

#include <iostream>
#include <stdio.h>

using namespace std;



int main()
{
    Str s1 = NullStr;
    Str s2 = Str("General");
    Str s3;
    Str s4, s5, s6;

    s3 = NullStr;
    s4 = Str("Sandman");

    setStrNull(&s5);
    setStr(&s6, "Sandman");

    Str s7, s8;
    char *a = "hello world";
    setStr(&s7, a);
    setStr(&s8, "hello world");

    

    printStr(&s1);
    printStr(&s2);
    printStr(&s3);
    printStr(&s4);
    printStr(&s5);
    printStr(&s6);
    printStr(&s7);
    printStr(&s8);

    return 0;
}