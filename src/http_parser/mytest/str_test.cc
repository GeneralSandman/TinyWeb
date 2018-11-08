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
#include<string>

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


    Str str5;
    
    {
        char *tmp = new char[8];
        tmp = "zhenhuli";
        int len = 4;
        setStrn(&str5, tmp, len);
        printStr(&str5);
        delete tmp;
    }

    printStr(&str5);
    //core dump because delete malloc-memory

    return 0;
}
