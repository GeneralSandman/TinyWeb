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

#include "http_model_file.h"

#include <iostream>
#include <string>
#include <string.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int initFile(File *file, const std::string &fname)
{
    file->name = fname;
    file->offset = 0;

    int return_val = stat(file->name.c_str(), &(file->info));
    if (return_val < 0)
    {
        //print_error("open html file error");//TODO
        std::cout << "[Debug] responser get file (" << file->name << ") stat failed" << std::endl;
        if (errno == ENOENT)
        {
            std::cout << "no such file\n"
                      << std::endl;
            return -1;
        }
        return -2;
    }

    return_val = open(file->name.c_str(), O_RDONLY);
    if (return_val < 0)
    {
        //print_error("open html file error");//TODO
        std::cout << "[Debug] responser open file (" << file->name << ") failed" << std::endl;
        if (errno == EACCES)
        {
            std::cout << "owner hasn't read permission\n";
            return -3;
        }
        return -4;
    }

    file->valid = true;
    file->fd = return_val;
    return 0;
}

void destoryFile(File *file)
{
    if (file->valid)
        close(file->fd);
}

int sendfile(int outFd, File *file)
{

    if (!S_ISREG(file->info.st_mode))
    {
        std::cout << "[Debug] responser file (" << file->name << ") is not regular file" << std::endl;
        return -1;
    }

    std::cout << "file size:" << file->info.st_size << std::endl;

    ssize_t res = sendfile(outFd, file->fd, &(file->offset), file->info.st_size);
    if (res < 0)
    {
        //print_error("open html file error");//TODO
        std::cout << "[Debug] responser sendfile (" << file->name << ") failed" << std::endl;
        printf("%s\n", strerror(errno));
    }
    std::cout << "[Debug] responser sendfile size =" << res << std::endl;

    return res;
}
