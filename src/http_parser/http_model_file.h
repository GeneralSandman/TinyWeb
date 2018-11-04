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

#ifndef HTTP_MODEL_FILE_H
#define HTTP_MODEL_FILE_H

#include <iostream>
#include <string>
#include <sys/stat.h>

typedef struct File
{
    std::string name;
    bool valid;
    int fd;
    struct stat info;
    off_t offset;
} File;

int initFile(File * file, const std::string &fname);
void destoryFile(File *file);

int sendfile(int outFd, File* file);

#endif // !HTTP_MODEL_FILE_H