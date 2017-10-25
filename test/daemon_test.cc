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

#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../api.h"

using namespace std;

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

int main()
{
    m_fSwitchtoDaemon();
    pause();
    return 0;
}