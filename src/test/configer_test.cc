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


#include <tiny_base/api.h>
#include <tiny_base/log.h>
#include <tiny_base/configer.h>

#include <iostream>

using namespace std;

void test1()
{
    std::string file;
    std::cin >> file;
    Configer &configer = Configer::getConfigerInstance();
    configer.setConfigerFile(file);

    if (0 == configer.loadConfig())
        std::cout << "load config successfull\n";
    else
        std::cout << "load config failed\n";
}

void test2()
{ //
    std::string a = "../TinyWeb.conf";
    setConfigerFile(a);
    if (loadConfig())
        std::cout << "load config successfull\n";
    else
        std::cout << "load config failed\n";

    // initLogger(debugfile,
    //            infofile,
    //            warnfile,
    //            errorfile,
    //            fatalfile,
    //            convertStringToLoglevel(loglevel)); //error used
    // //FIXME:
    LOG(Debug) << endl;
    LOG(Info) << endl;
    LOG(Warn) << endl;
    LOG(Error) << endl;
    LOG(Fatal) << endl;

    setDebugLogFile("/home/li/TinyWeb/log/TinyWeb.log");
    setInfoLogFile("/home/li/TinyWeb/log/TinyWeb.log");
    setWarnLogFile("/home/li/TinyWeb/log/TinyWeb.log");
    setErrorLogFile("/home/li/TinyWeb/log/TinyWeb.log");
    setFatalLogFile("/home/li/TinyWeb/log/TinyWeb.log");

    LOG(Debug) << "test configer_test" << endl;
    LOG(Info) << "test configer_test" << endl;
    LOG(Warn) << "test configer_test" << endl;
    LOG(Error) << "test configer_test" << endl;
    LOG(Fatal) << "test configer_test" << endl;
}

int main()
{
    test1();
    return 0;
}