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

#include <tiny_base/file.h>

#include <string>
#include <vector>

class HttpFile : public File {
public:
    std::string mime_type;

public:
    HttpFile()
        : File()
    {
    }
    virtual ~HttpFile() {}

    virtual int setFile(const std::string& fname);
    int setPathWithDefault(const std::string& path, const std::vector<std::string>& pages);
};

#endif // !HTTP_MODEL_FILE_H
