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

#include "../reader.h"

#include <iostream>
#include <string>

using namespace std;

int main()
{
    const std::string file = "../TinyWeb.conf";
    Reader reader(file);
    cout << "line number:" << reader.getLineNu() << endl;

    while (reader.haveMore())
    {
        cout << reader.getCurrLineNu() << ":" << reader.readLine();
    }

    cout << "reader again\n";
    reader.resetToHead();

    while (reader.haveMore())
    {
        cout << reader.getCurrLineNu() << ":" << reader.readLine();
    }
    //------------------------------------//
    cout << "change reader file\n";
    reader.setFile(file);

    cout << "line number:" << reader.getLineNu() << endl;

    while (reader.haveMore())
    {
        cout << reader.getCurrLineNu() << ":" << reader.readLine();
    }

    cout << "reader again\n";
    reader.resetToHead();

    while (reader.haveMore())
    {
        cout << reader.getCurrLineNu() << ":" << reader.readLine();
    }
}