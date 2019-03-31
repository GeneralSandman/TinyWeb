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

#include <TinyWebConfig.h>
#include <tiny_base/api.h>
#include <tiny_base/configer.h>
#include <tiny_core/processpool.h>
#include <tiny_http/http_model_file.h>

#include <getopt.h>
#include <iostream>
#include <map>
#include <unistd.h>

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

// FLAGS: read & write; create file if not exist.
#define FLAGS O_RDWR | O_CREAT
#define MODE S_IRWXU | S_IXGRP | S_IROTH | S_IXOTH

pid_t m_fSwitchtoDaemon()
{
    // How to create daemon process
    // https://en.wikipedia.org/wiki/Daemon_(computing)
    pid_t pid;

    // First step: fork and kill parent process.
    pid = fork();
    if (pid > 0) {
        std::cout << "parent-process(" << getpid() << ") exit\n";
        exit(0);
    } else if (pid < 0)
        handle_error("fork error\n");

    // Secode step:
    // create a new session,
    // becoming the session leader of this new session,
    // becoming the process group leader.
    setsid();

    // Third step:
    // dissociating form the controlling tty
    // this means that it is no longer a session leader
    pid = fork();
    if (pid > 0) {
        std::cout << "parent2-process(" << getpid() << ") exit\n";
        exit(0);
    } else if (pid < 0)
        handle_error("fork error\n");

    pid_t daemon_pid = getpid();
    std::cout << "daemon pid:" << daemon_pid << std::endl;

    // Forth step:
    // setting the root directory.
    chdir("/");

    // Fifth step:
    // changing the umask to 0.
    umask(0);

    // Sixth step:
    // close all file descriptors which inhert from parent(not include stardard streams)
    int Max = getdtablesize();
    for (int i = 3; i <= Max; i++)
        close(i);

    // Seventh step:
    // redirecting the standard streams to /dev/null
    int fd = open("/dev/null", O_RDWR, 0);
    if (fd == -1) {
        handle_error("redirect standard streams error");
    }

    dup2(fd, STDIN_FILENO);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
    close(fd);

    return daemon_pid;
}

int getTinyWebOpt(int argc, char** argv, std::map<char, std::string>& opt)
{
    struct option longopts[] = {
        { "configfile", required_argument, NULL, 'c' },
        { "order", required_argument, NULL, 'o' },
        { "tcf", required_argument, NULL, 't' },
        { "debug", no_argument, NULL, 'd' },
        { "version", no_argument, NULL, 'v' },
        { 0, 0, 0, 0 },
    };
    const char* short_options = "c:o:t::dv";

    opt = getOption(argc, argv, longopts, short_options);
    if (opt.size() == 0) {
        std::cout << "tinyweb command-line argv error\n";
        return 1;
    }
    return 0;
}

int getNamebyPid(pid_t pid, std::string& name)
{
    char file[1024];
    sprintf(file, "/proc/%d/status", pid);

    // get pid from pid file
    int fd;
    char buf[64];
    memset((void*)buf, 0, 64);
    if ((fd = open(file, O_RDONLY, MODE)) == -1) {
        printf("open process-status file(%s) error\n", file);
        name = "";
        return 1;
    }
    ssize_t read_d = read(fd, (void*)buf, 64);
    if (read_d == -1) {
        std::cout << "no content" << std::endl;
        return 1;
    }
    buf[read_d] = '\0';

    char pre[32];
    char name_s[32];
    int return_val = sscanf(buf, "%s\t%s", pre, name_s);
    name = std::string(name_s);

    close(fd);
    return 0;
}

bool daemaonStillRun(std::string& name, pid_t prepid)
{
    if (name == "")
        name = "TinyWeb";
    std::string oldname;
    if (0 != getNamebyPid(prepid, oldname))
        return false;

    if (oldname == name)
        return true;
    else
        return false;
}

pid_t daemonPrePid()
{
    // pid = get pid from pid file
    std::string pidfile = "/var/run/TinyWeb.pid";
    int fd;
    char readbuf[1024];

    if ((fd = open(pidfile.c_str(), FLAGS, MODE)) == -1) {
        printf("open pidfile(%s) error\n", pidfile.c_str());
        return 0;
    }
    memset((void*)readbuf, 0, 1024);
    ssize_t read_d = read(fd, (void*)readbuf, 1024);
    if (read_d == -1)
        std::cout << "no content" << std::endl;
    readbuf[read_d] = '\0';
    pid_t prepid = 0;
    prepid = atoi(readbuf);

    std::string name = "main_test";
    bool stillRun = daemaonStillRun(name, prepid);
    if (!stillRun) {
        prepid = 0;
    }
    close(fd);
    return prepid;
}

int storeCurPid(pid_t curpid)
{
    std::string pidfile = "/var/run/TinyWeb.pid";
    int fd;
    if ((fd = open(pidfile.c_str(), FLAGS, MODE)) == -1) {
        printf("open pidfile(%s) error\n", pidfile.c_str());
        return 0;
    }
    int return_val = ftruncate(fd, 0);
    if (0 != return_val)
        std::cout << "ftruncate error" << std::endl;
    lseek(fd, 0, SEEK_SET);
    std::string writebuf = std::to_string(curpid);
    ssize_t write_d = write(fd, (void*)(writebuf.c_str()), writebuf.size());
    close(fd);
    return 0;
}

void initConfiger(const std::string& file, bool debug)
{
    Configer& configer = Configer::getConfigerInstance();

    if (0 == configer.checkConfigerFile(file)) {
        LOG(Info) << "configure-file(" << file << ") valid\n";
        configer.setConfigerFile(file);
    } else {
        LOG(Info) << "configure-file(" << file << ") invalid\n";
        return;
    }

    if (0 == configer.loadConfig(debug)) {
        LOG(Info) << "load configure-file(" << file << ") successfully\n";
    } else {
        LOG(Info) << "load configure-file(" << file << ") unsuccessfully\n";
    }
}

void initLogger(bool debug)
{
    Configer& configer = Configer::getConfigerInstance();
    LogConfig log = configer.getLogConfig();

    std::string loglevel = log.level;
    std::string logpath = log.path;
    std::string debugfile = log.path + debugfile;
    std::string infofile = log.path + infofile;
    std::string warnfile = log.path + warnfile;
    std::string errorfile = log.path + errorfile;
    std::string fatalfile = log.path + fatalfile;
    initLogger(debugfile,
        infofile,
        warnfile,
        errorfile,
        fatalfile,
        convertStringToLoglevel(loglevel)); //error used

    if (debug)
        setLogLevel(Debug);
}

int startWork(const std::string& config_file, bool debug)
{
    initConfiger(config_file, debug);
    initLogger(debug);

    ProcessPool pool;
    pool.init();
    pool.createProcess(4);

    pool.start();
}

int main(int argc, char** argv)
{

    std::map<char, std::string> opt;
    if (0 != getTinyWebOpt(argc, argv, opt)) {
        return 1;
    }

    // -v
    // Just get version of TinyWeb.
    if (opt.end() != opt.find('v')) {
        std::cout << "TinyWeb version: " << TINYWEB_VERSION << std::endl;

        return 0;
    }

    // --tcf
    // Just check configure file is valid or not.
    if (opt.end() != opt.find('t')) {
        std::cout << "TinyWeb check configfile:" << opt['t'] << std::endl;
        Configer& configer = Configer::getConfigerInstance();
        if (0 == configer.checkConfigerFile(opt['t'])) {
            std::cout << "TinyWeb configure file(" << opt['t'] << ") is valid" << std::endl;
        } else {
            std::cout << "TinyWeb configure file(" << opt['t'] << ") is invalid" << std::endl;
        }

        return 0;
    }

    pid_t prepid = daemonPrePid();
    std::cout << "pre pid:" << prepid << std::endl;

    // -o <order>
    // Just order process.
    if (opt.end() != opt.find('o')) {
        if (prepid == 0) {
            std::cout << "There is not TinyWeb process running\n";
            return 1;
        }

        std::string order = opt['o'];
        if (order == "stop") {
            std::cout << "stop TinyWeb(" << prepid << ")" << std::endl;
            kill(prepid, SIGQUIT);
        } else if (order == "term") {
            std::cout << "term TinyWeb(" << prepid << ")" << std::endl;
            kill(prepid, SIGTERM);
            // kill(prepid, SIGINT);
        } else if (order == "restart") {
            std::cout << "restart TinyWeb(" << prepid << ")" << std::endl;
            kill(prepid, SIGUSR1);
        } else if (order == "reload") {
            std::cout << "reload TinyWeb(" << prepid << ")" << std::endl;
            kill(prepid, SIGUSR2);
        } else {
            std::cout << "unknow order" << std::endl;
        }

        return 0;
    }

    // -d -c <configure-file>
    // Start worker process.
    if (prepid) {
        std::cout << "TinyWeb already running\n";
        return 0;
    } else {
        pid_t pid = getpid();
        bool debug = true;

        if (opt.end() != opt.find('c'))
            std::cout << "configfile:" << opt['c'] << std::endl;

        if (opt.end() == opt.find('d')) {
            pid = m_fSwitchtoDaemon();
            debug = false;
            std::cout << "start new daemon process" << std::endl;
        }
        storeCurPid(pid);

        startWork(opt['c'], debug);
    }

    return 0;
}
