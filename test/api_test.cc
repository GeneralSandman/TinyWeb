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

#include "../api.h"

using namespace std;

int main()
{
    int a;
    cin >> a;
    if (a == 1)
    {
        char s[] = "error";
        handle_error(s);
    }
    if (a == 2)
    {
        string s = "error";
        handle_error_s(s);
    }
    if (a == 3)
    {
        handle_error("error");
    }
    if (a == 4)
    {
        handle_error_s("error");
    }
    return 0;
}