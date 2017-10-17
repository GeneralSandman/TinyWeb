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
    {
        std::string a = "../TinyWeb.conf";
        Configer &configer = Configer::getConfigerInstance();

        configer.setConfigerFile(a);

        if (configer.loadConfig())
            std::cout << "load config successfull\n";
        else
            std::cout << "load config failed\n";
        configer.test();
        cout << configer.getConfigValue("loglevel");
    }

    { //
        std::string a = "../TinyWeb.conf";
        setConfigerFile(a);
        if (loadConfig())
            std::cout << "load config successfull\n";
        else
            std::cout << "load config failed\n";
        Configer::getConfigerInstance().test();
        cout << getConfigValue("loglevel") << endl;

        std::string loglevel = getConfigValue("loglevel");
        std::string logpath = getConfigValue("logpath");
        std::string debugfile = logpath + getConfigValue("debugfile");
        std::string infofile = logpath + getConfigValue("infofile");
        std::string warnfile = logpath + getConfigValue("warnfile");
        std::string errorfile = logpath + getConfigValue("errorfile");
        std::string fatalfile = logpath + getConfigValue("fatalfile");

        std::cout << debugfile << std::endl;

        initLogger(debugfile,
                   infofile,
                   warnfile,
                   errorfile,
                   fatalfile,
                   convertStringToLoglevel(loglevel)); //error used
        //FIXME:
        LOG(Debug) << endl;
        LOG(Info) << endl;
        LOG(Warn) << endl;
        LOG(Error) << endl;
        LOG(Fatal) << endl;
    }

    return 0;
}