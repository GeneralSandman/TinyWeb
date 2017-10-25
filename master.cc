/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

#include "master.h"
#include "eventloop.h"
#include "server.h"
#include "protocol.h"
#include "configer.h"
#include "factory.h"
#include "log.h"
#include "api.h"

#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

EventLoop *Master::m_pEventLoop = nullptr;

void Master::m_fSwitchtoDaemon()
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

void Master::m_fInit()
{
    //first step:
    //add signal handler
    add_signal(SIGTERM, m_fSignalHandler);
    add_signal(SIGINT, m_fSignalHandler);
}

void Master::m_fSignalHandler(int sig)
{
    if (m_pEventLoop)
        m_pEventLoop->quit();
}

Master::Master(const std::string &configfile)
{
    //init configer
    m_nConfigFile = configfile;
    setConfigerFile(configfile);
    loadConfig();

    //get log args and init log
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

    //change to daemon process
    if (!(Debug == convertStringToLoglevel(loglevel)))
        m_fSwitchtoDaemon();

    //init listen Address
    std::string listen = getConfigValue("listen");
    int listenPort = atoi(listen.c_str());
    m_nAddress = NetAddress(listenPort);

    //init member
    if (!m_pEventLoop)
        m_pEventLoop = new EventLoop();
    WebProtocol prot;
    m_pFactory = new Factory(m_pEventLoop, prot);
    m_pServer = new Server(m_pEventLoop, m_nAddress, m_pFactory);

    m_fInit();
    LOG(Debug) << "class Master constructor\n";
}

void Master::start()
{
    m_pServer->start();
    m_pEventLoop->loop();
}

Master::~Master()
{
    delete m_pEventLoop;
    delete m_pFactory;
    delete m_pServer;

    m_pEventLoop = nullptr;
    m_pFactory = nullptr;
    m_pServer = nullptr;

    LOG(Debug) << "class Master destructor\n";
}