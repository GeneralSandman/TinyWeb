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

#include <tiny_base/log.h>
#include <tiny_base/memorypool.h> 
#include <tiny_http/http_model_file.h>

#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <string.h>
#include <string>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

int isRegularFile(const std::string& fname)
{
    struct stat info;
    int return_val;

    return_val = stat(fname.c_str(), &info);
    if (return_val < 0) {
        // std::cout << "[Debug] responser get file (" << fname << ") stat failed" << std::endl;
        if (errno == ENOENT) {
            // std::cout << "no such file or directory"
            // << std::endl;
        }
        return -1;
    }

    if (S_ISDIR(info.st_mode)) {
        return 1;
    }

    if (S_ISREG(info.st_mode)) {
        return 0;
    }

    return return_val;
}

std::string getType(const std::string& f)
{
    std::string res;
    bool findPoint = false;
    for (int i = f.size() - 1; i >= 0; i--) {
        if (f[i] == '.') {
            findPoint = true;
            break;
        }
        if (f[i] == '/')
            break; //encounter a slash
        res = f[i] + res;
    }
    if (!findPoint)
        res = "";
    return res;
}

std::string getMimeType(const std::string& type)
{
    //TODO: change code
    std::string res;

    if ("html" == type) {
        res = "text/html";
    } else if ("htm" == type) {
        res = "text/htm";
    } else if ("css" == type) {
        res = "text/css";
    } else if ("js" == type) {
        res = "application/javascript";
    } else if ("bmp" == type) {
        res = "image/bmp";
    } else if ("gif" == type) {
        res = "image/gif";
    } else if ("jpeg" == type) {
        res = "image/jpeg";
    } else if ("png" == type) {
        res = "image/png";
    }

    return res;
}

int HttpFile::setFile(const std::string& fname)
{
    name = fname;
    offset = 0;
    valid = false;

    int return_val = stat(name.c_str(), &info);
    if (return_val < 0) {
        if (errno == ENOENT) {
        }
        return -1;
    }

    return_val = open(name.c_str(), O_RDONLY);
    if (return_val < 0) {
        if (errno == EACCES) {
            // std::cout << "owner hasn't read permission\n";
        }
        return -1;
    }

    type = getType(name);
    mime_type = getMimeType(type);
    valid = true;
    fd = return_val;

    LOG(Debug) << "response file:" << name << std::endl;
    return 0;
}

int HttpFile::setPathWithDefault(const std::string& path, const std::vector<std::string>& pages)
{
    int return_val;
    std::string tmp_file_name;

    for (auto t : pages) {
        tmp_file_name = path + "/" + t;

        return_val = setFile(tmp_file_name);
        if (!return_val) {
            return 0;
        }
    }

    return -1;
}

void putData(chain_t* chain)
{
    if (!valid || chain == nullptr) {
        return;
    }

    unsigned int chain_size;
    chain_t* tmp_chain;
    buffer_t* tmp_buffer;

    chain_size = 0;
    tmp_chain = chain;
    while(tmp_chain) {
        tmp_buffer = tmp_chain->buffer;
        chain_size += tmp_buffer->end - tmp_buffer->begin;

        tmp_chain = tmp_chain->next;
    }

    LOG(Debug) << "file size(" << info.st_size << "),"
               << "chain size(" << chain_size << ")\n";
}

HttpFile::~HttpFile()
{
    if (valid)
        close(fd);
}

// int sendfile(int outFd, HttpFile* file)
// {
//     if (outFd == 0) {
//         LOG(Debug) << "[model file] sendfile " << file->name << std::endl;
//         return 1;
//     }

//     // TODO: Change this code to tiny_core_model.
//     ssize_t res = sendfile(outFd, file->fd, &(file->offset), file->info.st_size);
//     if (res < 0) {
//         //print_error("open html file error");//TODO
//         std::cout << "[Debug] responser sendfile (" << file->name << ") failed" << std::endl;
//         printf("%s\n", strerror(errno));
//     }
//     //std::cout << "[Debug] responser sendfile size =" << res << std::endl;

//     return res;
// }
