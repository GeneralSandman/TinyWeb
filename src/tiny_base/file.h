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

#ifndef FILE_H
#define FILE_H

#include <tiny_base/memorypool.h>

#include <sys/stat.h>

int isRegularFile(const std::string& fname);
std::string getType(const std::string& f);

class File {
public:
    std::string name;
    std::string type;
    bool valid;
    int fd;
    off_t offset;
    struct stat info;

    // appendData only used by getData(),
    // It append data begin dest and
    // Return the end of chain.
    unsigned int appendData(chain_t* & dest, const char* data, unsigned int len);

public:
    File()
        : valid(false)
        , fd(-1)
        , offset(0)
    {
        memset((void*)&info, 0, sizeof(info));
    }

    int setFile(const std::string& fname);
    inline unsigned int getFileSize()
    {
        if (valid) {
            return info.st_size;
        }
        return 0;
    }
    inline bool noMoreData()
    {
        if (valid) {
            // std::cout << "noMoreData:offset(" << offset << "),file-size(" << info.st_size << ")\n";
            return (offset == info.st_size);
        } else {
            return true;
        }
        return false;
    }

    void getData(chain_t* chain);
    // using for Range && Content-Range.
    void getData(chain_t* chain, off_t begin, off_t end);

    int writeData(chain_t* chain);

    ~File()
    {
        if (valid) {
            close(fd);
        }
    }
};

#endif // !FILE_H
