#include "../log.h"
#include <ctime>
#include <iostream>
#include <unistd.h>
using namespace std;

int main()
{
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
    std::cout << "main done\n";

    return 0;
}