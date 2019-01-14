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

#include <tiny_base/configer.h>
#include <tiny_base/log.h>
#include <tiny_base/memorypool.h>
#include <tiny_base/file.h> 
#include <tiny_http/http_model_file.h>

#include <errno.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>

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
        return -2;
    }

    type = getType(name);
    Configer& configer = Configer::getConfigerInstance();
    mime_type = configer.getMimeType(type);
    std::cout << "type:" << type << ",mime_type:" << mime_type << std::endl;
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
        if (0 == return_val) {
            return 0;
        }
    }

    return -1;
}

