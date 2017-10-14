/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

#include "../log.h"
#include "../api.h"
#include "../configer.h"

#include <iostream>

using namespace std;

int main()
{
    // {
    //     std::string a = "../TinyWeb.conf";
    //     Configer &configer = Configer::getConfigerInstance();

    //     configer.setConfigerFile(a);

    //     if (configer.loadConfig())
    //         std::cout << "load config successfull\n";
    //     else
    //         std::cout << "load config failed\n";
    //     configer.test();
    //     cout << configer.getConfigValue("loglevel");
    // }

    {//
        std::string a = "../TinyWeb.conf";
        setConfigerFile(a);
        if (loadConfig())
            std::cout << "load config successfull\n";
        else
            std::cout << "load config failed\n";
        Configer::getConfigerInstance().test();
        cout << getConfigValue("loglevel");
    }

    return 0;
}