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

#include <iostream>
#include <cstring>

#define f(s) sizeof(s)

using namespace std;

void str(char *)
{
    std::cout << "no const\n";
}

void str(const char *)
{
    std::cout << "const\n";
}

void test1()
{

    int *a;
    std::cout << sizeof(a) << std::endl; //8

    char *str = "11";
    std::cout << sizeof(str) << std::endl //8
              << strlen(str) << std::endl //2
              << f(str) << std::endl;     //8

    char s[] = "11";
    std::cout << sizeof(s) << std::endl //3
              << strlen(s) << std::endl //2
              << f(s) << std::endl;     //3

    str = "111";
    // s = "22";//error
}

void test2()
{

    char *s1 = "11";
    char s[] = "11";

    str(s1);
    str((const char *)s);
}

void testing(char str[])
{
    std::cout << "array\n";
}

void testing(const char *str)
{
    std::cout << "const ptr\n";
}

#include "../string_t.h"
int main()
{

    char *str1 = "123456789";
    char str2[] = "987654321";

    // testing((const char*)str1);
    // testing(str2);

    String s1;
    String s2(str1, 2);
    String s3(str2, 2);
    String s4(str1);
    String s5(str2);

    s2.setNull();
    s3 = s2;
    s4.swap(s5);

    if (s3 > s4)
        std::cout << "s3>s4\n";

    return 0;
}