/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

#include "../log.h"
#include "../api.h"
#include "../config.h"

#include <iostream>

using namespace std;

int main()
{
    {
        Configer conf("../TinyWeb.conf");
        if (conf.loadConfig())
            std::cout << "load config successfull\n";
        else
            std::cout << "load config failed\n";
        conf.test();
        cout << conf.getValue("loglevel");
    }

    return 0;
}