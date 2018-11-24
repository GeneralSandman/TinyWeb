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

#include <tiny_core/server.h>
#include <tiny_core/eventloop.h>
#include <tiny_core/netaddress.h>
#include <tiny_core/connection.h>
#include <tiny_core/protocol.h>
#include <tiny_core/factory.h>
#include <tiny_core/time.h>
#include <tiny_base/configer.h>
#include <tiny_base/buffer.h>
#include <tiny_base/api.h>
#include <tiny_base/log.h>

#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

EventLoop *g_loop = nullptr;

void m_fSwitchtoDaemon()
{
    //how to create daemon process
    //https://en.wikipedia.org/wiki/Daemon_(computing)
    pid_t pid;

    //First step:fork and end parent process
    pid = fork();
    if (pid > 0)
    {
        exit(0);
    }
    else if (pid < 0)
        handle_error("fork error\n");

    //Secode step:
    //create a new session and
    //becoming the session leader of that new session and
    //becoming the process group leader
    setsid();

    //Third step:
    //dissociating form the controlling tty
    //this means that it is no longer a session leader
    pid = fork();
    if (pid > 0)
    {
        exit(0);
    }
    else if (pid < 0)
        handle_error("fork error\n");
    std::cout << "pid:" << getpid() << std::endl;

    //Forth step:
    //setting the root directory
    chdir("/");

    //Fifth step:
    //changing the umask to 0
    umask(0);

    //Sixth step:
    //close all file descriptors which inhert from parent(not include stardard streams)
    int Max = getdtablesize();
    for (int i = 3; i <= Max; i++)
        close(i);

    //Seventh step:
    //redirecting the standard streams to /dev/null
    int fd = open("/dev/null", O_RDWR, 0);
    if (fd == -1)
    {
        handle_error("redirect standard streams error");
    }
    dup2(fd, STDIN_FILENO);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
    close(fd);
}

static void m_fSignalHandler(int sig)
{
    if (g_loop)
        g_loop->quit();
}

void m_fInit()
{
    //first step:
    //add signal handler
    add_signal(SIGTERM, m_fSignalHandler);
    add_signal(SIGINT, m_fSignalHandler);
}

void fun1()
{
    LOG(Info) << "info-\n";
}

int main()
{
    m_fSwitchtoDaemon();

    //config
    std::string configeFile = "/home/li/TinyWeb/TinyWeb.conf";
    setConfigerFile(configeFile);
    loadConfig();

    //log
    std::string loglevel = getConfigValue("loglevel");
    std::string logpath = getConfigValue("logpath");
    std::string debugfile = logpath + getConfigValue("debugfile");
    std::string infofile = logpath + getConfigValue("infofile");
    std::string warnfile = logpath + getConfigValue("warnfile");
    std::string errorfile = logpath + getConfigValue("errorfile");
    std::string fatalfile = logpath + getConfigValue("fatalfile");

    initLogger(debugfile,
               infofile,
               warnfile,
               errorfile,
               fatalfile,
               convertStringToLoglevel(loglevel)); //error used

    //signal

    setLogLevel(Debug);

    add_signal(SIGTERM, m_fSignalHandler);
    add_signal(SIGINT, m_fSignalHandler);

    g_loop = new EventLoop();
    g_loop->runEvery(1, boost::bind(fun1));

    NetAddress address("127.0.0.1:80");

    WebProtocol prot;
    Factory *factory = new Factory(g_loop, prot);
    Server server(g_loop, address, createNoBlockSocket(), factory);

    server.start();

    g_loop->loop();
    delete g_loop;
    delete factory;
    return 0;
}