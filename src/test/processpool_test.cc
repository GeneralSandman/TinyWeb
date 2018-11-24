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

#include <tiny_core/processpool.h>

#include <iostream>

using namespace std;

int main()
{
    {
        std::string loglevel = "Debug";
        std::string logpath = "/home/dell/TinyWeb/log/";
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
        pool.createProcess(2);
    }

    return 0;
}
