/*
 *Author:GeneralSandman
 *Code:https://github.com/GeneralSandman/TinyWeb
 *E-mail:generalsandman@163.com
 *Web:www.dissigil.cn
 */

/*---XXX---
 *
 ****************************************
 *
 */

#include <tiny_base/configer.h> 
#include <tiny_core/processpool.h>

#include <iostream>

using namespace std;

void initConfiger()
{
    std::string file = "../../TinyWeb.conf";
    Configer& configer = Configer::getConfigerInstance();
    configer.setConfigerFile(file);

    bool debug = true;
    if (0 == configer.loadConfig(debug))
        std::cout << "++load config successfull\n";
    else
        std::cout << "--load config failed\n";
    std::cout << std::endl;
}

int main()
{
    initConfiger();
    {
        std::string loglevel = "Debug";
        std::string logpath = "/home/tinyweb/log/";
        std::string debugfile = logpath + "debug.log";
        std::string infofile = logpath + "info.log";
        std::string warnfile = logpath + "warn.log";
        std::string errorfile = logpath + "error.log";
        std::string fatalfile = logpath + "fatal.log";
        initLogger(debugfile,
            infofile,
            warnfile,
            errorfile,
            fatalfile,
            convertStringToLoglevel(loglevel)); //error used
        setLogLevel(Debug);

        std::cout << "[Process Pool Test]" << std::endl;
        ProcessPool pool;
        pool.init();
        pool.createProcess(1);
    }

    return 0;
}
