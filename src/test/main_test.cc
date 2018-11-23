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

#include <tiny_base/api.h>
#include <tiny_core/processpool.h>
#include <tiny_http/http_model_file.h>
#include <TinyWebConfig.h>

#include <iostream>
#include <unistd.h>
#include <getopt.h>
#include <map>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>

//定义flags:只写，文件不存在那么就创建，文件长度戳为0
#define FLAGS O_RDWR | O_CREAT
#define FLAGS1 O_WRONLY | O_CREAT | O_TRUNC
//创建文件的权限，用户读、写、执行、组读、执行、其他用户读、执行
#define MODE S_IRWXU | S_IXGRP | S_IROTH | S_IXOTH

int main(int argc, char **argv)
{
    std::map<char, std::string> opt;
    struct option longopts[] = {
        {"configfile", required_argument, NULL, 'c'},
        {"order", required_argument, NULL, 'o'},
        {"tcf", required_argument, NULL, 't'},
        {"debug", no_argument, NULL, 'd'},
        {"version", no_argument, NULL, 'v'},
        {0, 0, 0, 0},
    };
    const char *short_options = "c:o:t::dv";

    opt = getOption(argc, argv, longopts, short_options);
    if (opt.size() == 0)
    {
        std::cout << "argv error\n";
        exit(-1);
    }

    if (opt.end() != opt.find('c'))
        std::cout << "configfile:" << opt['c'] << std::endl;

    if (opt.end() != opt.find('o'))
        std::cout << "order:" << opt['o'] << std::endl;

    if (opt.end() != opt.find('t'))
        std::cout << "testconfigfile:" << opt['t'] << std::endl;

    if (opt.end() != opt.find('d'))
        std::cout << "run as debug" << std::endl;
    if (opt.end() != opt.find('v'))
        std::cout << "TinyWeb ersion: " << TINYWEB_VERSION << std::endl;

    // pid = get pid from pid file
    std::string pidfile = "/var/run/TinyWeb.pid";
    int fd;
    if ((fd = open(pidfile.c_str(), FLAGS, MODE)) == -1)
    {
        printf("open pidfile error\n");
        return 0;
    }

    std::cout << "current pid:" << getpid() << std::endl;
    char buf[1024];
    memset((void *)buf, 0, 1024);
    ssize_t read_d = read(fd, (void *)buf, 1024);
    if (read_d == -1)
    {
        std::cout << "no content" << std::endl;
    }
    buf[read_d] = '\0';
    pid_t prepid = atoi(buf);
    std::cout << "pre pid:" << prepid << std::endl;

    // clear pid file
    ssize_t write_d;
    int return_val = ftruncate(fd, 0);
    if (0 == return_val)
    {
        std::cout << "ftruncate success" << std::endl;
    }
    lseek(fd, 0, SEEK_SET);

    // write to pid file
    std::string writebuf = std::to_string(getpid());
    write_d = write(fd, (void *)(writebuf.c_str()), writebuf.size());
    if (-1 == write_d)
    {
        std::cout << "write pid to pidfile error" << std::endl;
    }
    close(fd);

    if (opt.end() != opt.find('o') && prepid != 0)
    {
        std::string order = opt['o'];
        std::cout << "order:" << order << std::endl;

        if (order == "stop")
        {
            std::cout << "stop TinyWeb" << std::endl;
            kill(prepid, SIGQUIT);
        }
        else if (order == "term")
        {
            std::cout << "term TinyWeb" << std::endl;
            kill(prepid, SIGTERM);
            // kill(prepid, SIGINT);
        }
        else if (order == "restart")
        {
            std::cout << "restart TinyWeb" << std::endl;
            kill(prepid, SIGUSR1);
        }
        else if (order == "reload")
        {
            std::cout << "reload TinyWeb" << std::endl;
            kill(prepid, SIGUSR2);
        }
        else
        {
            std::cout << "unknow order" << std::endl;
        }
    }

    if (0 == prepid)
    {
        while(1)
        {
            // sleep(5);
        }
        // std::cout << "[Process Pool Test]" << std::endl;
        // ProcessPool pool;
        // pool.init();
        // pool.createProcess(2);
    }
    else
    {
        std::cout << "no need create new processpool" << std::endl;
    }

    //clear pid file

    return 0;
}