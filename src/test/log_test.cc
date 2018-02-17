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

#include <tiny_base/log.h>

#include <ctime>
#include <iostream>
#include <unistd.h>
using namespace std;

void test1()
{
    initLogger("/home/li/TinyWeb/log/debug.log",
               "/home/li/TinyWeb/log/info.log",
               "/home/li/TinyWeb/log/warn.log",
               "/home/li/TinyWeb/log/error.log",
               "/home/li/TinyWeb/log/fatal.log",
               Debug);
    for (log_level l = Debug; l <= Fatal; l = log_level(l + 1))
    {
        setLogLevel(l);

        LOG(Debug) << endl;
        LOG(Info) << endl;
        LOG(Warn) << endl;
        LOG(Error) << endl;
        LOG(Fatal) << endl;

        sleep(2);
    }
}

void test2()
{

    setDebugLogFile("/home/li/TinyWeb/log/TinyWeb.log");
    setInfoLogFile("/home/li/TinyWeb/log/TinyWeb.log");
    setWarnLogFile("/home/li/TinyWeb/log/TinyWeb.log");
    setErrorLogFile("/home/li/TinyWeb/log/TinyWeb.log");
    setFatalLogFile("/home/li/TinyWeb/log/TinyWeb.log");

    for (log_level l = Debug; l <= Fatal; l = log_level(l + 1))
    {
        setLogLevel(l);
        LOG(Debug) << endl;
        LOG(Info) << endl;
        LOG(Warn) << endl;
        LOG(Error) << endl;
        LOG(Fatal) << endl;
        sleep(2);
    }
}

void test3()
{
    //multi process test
    setDebugLogFile("/home/li/TinyWeb/log/TinyWeb.log");
    setInfoLogFile("/home/li/TinyWeb/log/TinyWeb.log");
    setWarnLogFile("/home/li/TinyWeb/log/TinyWeb.log");
    setErrorLogFile("/home/li/TinyWeb/log/TinyWeb.log");
    setFatalLogFile("/home/li/TinyWeb/log/TinyWeb.log");

    pid_t pid = fork();
    if (pid < 0)
    {
        std::cout << "fork error\n";
    }
    else if (pid == 0)
    {
        //child
        for (log_level l = Debug; l <= Fatal; l = log_level(l + 1))
        {
            setLogLevel(l);
            LOG(Debug) << "84----------------" << std::endl;
            LOG(Info) << "85================" << endl;
            LOG(Warn) << "86+++++++++++++++++" << std::endl;
            LOG(Error) << "87----------------" << std::endl;
            LOG(Fatal) << "88***************" << std::endl;
        }
    }
    else
    {
        //parent
        for (log_level l = Debug; l <= Fatal; l = log_level(l + 1))
        {
            setLogLevel(l);
            LOG(Debug) << "97^^^^^^^^^^^^^^^" << std::endl;
            LOG(Info) << "98%%%%%%%%%%%%%%%" << std::endl;
            LOG(Warn) << "99///////////////" << std::endl;
            LOG(Error) << "100!!!!!!!!!!!!!" << std::endl;
            LOG(Fatal) << "101#############" << std::endl;
        }
    }
}

int main()
{
    // test2();
    test3();
    std::cout << "main done\n";

    return 0;
}