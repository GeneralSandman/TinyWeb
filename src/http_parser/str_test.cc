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

#include"str_t.h"

#include<iostream>

using namespace std;

int main()
{
    
    Str str1 = Str("helloworld");
    Str str2 = Str("zhenhuli");

    printStr(&str1);
    printStr(&str2);

    
    Str str3;
    setStr(&str3, "zhenhuli");
    printStr(&str3);
    
    setStr(&str3, "helloworld");
    printStr(&str3);


    Str str4;
    
    setStr(&str4, "zhenhuli");
    setStrNull(&str4);
    printStr(&str4);


    //error using
    //compiler error

    return 0;
}
