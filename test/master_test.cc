/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

#include "../master.h"

#include <iostream>
using namespace std;

int main(int argc, char **argv)
{
    {
        string config_file;

        Master master(config_file);
        master.start();
    }

    return 0;
}