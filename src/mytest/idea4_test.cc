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

#include <cstring>
#include <vector>
#include <map>
#include <unordered_map>
#include <iostream>
#include <sys/stat.h>

struct cmp_str
{
    bool operator()(char const *a, char const *b)
    {
        return std::strcmp(a, b) < 0;
    }
};


int main()
{
    std::string filename = "/home/tinyweb/src/mytest/build/6/33/0d70dd3c42e1e544abe8f7c77a9ea0";
    std::vector<std::string> filenames = {
        ""
    }

    std::string tmp;
    std::vector<std::string> dirs;

    for (int i=0;i<filename.size();i++) {
        if (filename[i] == '/') {
            dirs.push_back(tmp);
            tmp = "";
        } else {
            tmp += filename[i];
        }
    }

    for (auto t:dirs) {
        std::cout << t << std::endl;
        mkdir(t.c_str(), S_IRWXG);
    }

}
