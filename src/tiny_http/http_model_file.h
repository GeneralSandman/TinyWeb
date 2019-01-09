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

#include <tiny_base/memorypool.h>

#include <iostream>
#include <string.h>
#include <string>
#include <sys/stat.h>
#include <vector>

int isRegularFile(const std::string& fname);
std::string getType(const std::string& f);
std::string getMimeType(const std::string& type);

class HttpFile {
public:
    std::string name;
    std::string type;
    std::string mime_type;
    bool valid;
    int fd;
    struct stat info;
    off_t offset;

public:
    HttpFile()
        : valid(false)
        , fd(-1)
        , offset(0)
    {
        memset((void*)&info, 0, sizeof(info));
    }

    int setFile(const std::string& fname);
    int setPathWithDefault(const std::string& path, const std::vector<std::string>& pages);

    unsigned int getFileSize()
    {
        if (valid) {
            return info.st_size;
        }
        return 0;
    }
    // appendData only used by getData(),
    // It append data begin dest and
    // Return the end of chain.
    chain_t* appendData(chain_t* dest, const char* data, unsigned int len);
    void getData(chain_t* chain);

    ~HttpFile();
};

// int sendfile(int outFd, HttpFile* file);

#endif // !HTTP_MODEL_FILE_H
