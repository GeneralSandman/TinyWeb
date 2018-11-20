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

#include <http_parser/http_time.h>

#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

using namespace std;

typedef struct testHttpTime
{
    time_t time;
    const char *str;
    const char *fmt;
    bool valid;
    unsigned int timestamp;
} testHttpTime;

testHttpTime times[] =
{
    {
        .time = 1541680751,
        .str = "Thu, 08 Nov 2018 12:39:11 GMT",
        .fmt = "%a, %d %b %Y %H:%M:%S %Z",
        .valid = true,
        .timestamp = 0,
    },
    {
        .time = 1541680705,
        .str = "Thu, 08 Nov 2018 12:38:25 GMT",
        .fmt = "%a, %d %b %Y %H:%M:%S %Z",
        .valid = true,
        .timestamp = 0,
    },
    {
        .time = 1541574037,
        .str = "Wed, 07 Nov 2018 07:00:37 GMT",
        .fmt = "%a, %d %b %Y %H:%M:%S %Z",
        .valid = true,
        .timestamp = 0,
    },
    {
        .time = 1541680684,
        .str = "Thu, 08 Nov 2018 12:38:04 GMT",
        .fmt = "%a, %d %b %Y %H:%M:%S %Z",
        .valid = true,
        .timestamp = 0,
    },
    {
        .time = 1542444072,
        .str = "Sat, 17 Nov 2018 08:41:12 GMT",
        .fmt = "%a, %d %b %Y %H:%M:%S %Z",
        .valid = true,
        .timestamp = 0,
    },
    {
        .time = 1542383480,
        .str = "Fri, 16 Nov 2018 15:51:20 GMT",
        .fmt = "%a, %d %b %Y %H:%M:%S %Z",
        .valid = true,
        .timestamp = 0,
    },
    {
        .time = 1542390346,
        .str = "Fri, 16 Nov 2018 17:45:46 GMT",
        .fmt = "%a, %d %b %Y %H:%M:%S %Z",
        .valid = true,
        .timestamp = 0,
    },
    {
        .time = 1541680744,
        .str = "Thu, 08 Nov 2018 12:39:04 GMT",
        .fmt = "%a, %d %b %Y %H:%M:%S %Z",
        .valid = true,
        .timestamp = 0,
    },
};

bool testTime(testHttpTime *t)
{
    //test formate
    formatTime formattime_tmp;
    formattime_tmp.fmt = std::string(t->fmt);

    struct tm gmt;
    convertTime2Gmt(&t->time, false, &gmt);
    formate(&gmt, &formattime_tmp);

    bool sameStr = (0 == strcmp(formattime_tmp.str.c_str(), t->str));
    if (!sameStr)
        std::cout << t->str << " " << formattime_tmp.str << std::endl;

    //test deformate
    time_t time_tmp;
    memset(&gmt, 0, sizeof(struct tm));
    memset(&time_tmp, 0, sizeof(time_tmp));

    deformate(&formattime_tmp, &gmt);
    convertTm2Time(&gmt, false, &time_tmp);

    bool sameTimet = (time_tmp == t->time);
    if (!sameTimet)
        std::cout << t->time << " " << time_tmp << std::endl;


    return sameStr && sameTimet;
}

void testPraseHttpTime()
{
    int alltest = 0;
    int passtest = 0;

    int len = sizeof(times) / sizeof(times[0]);

    for (int i = 0; i < 8; i++)
    {
        std::cout << i << ")" << std::endl;
        alltest++;

        bool res = testTime(times+i);

        if (res)
        {
            passtest++;
        }
    }

    std::cout << "[Parse HttpTime Test] pass/all = " << passtest << "/" << alltest << std::endl;
}



int readFileList(const std::string &basePath, std::vector<std::string> &files)
{
    DIR *dir;
    struct dirent *ptr;

    if ((dir = opendir(basePath.c_str())) == nullptr)
    {
        printf("open dir(%s) error\n", basePath.c_str());
        return 1;
    }

    while ((ptr = readdir(dir)) != nullptr)
    {
        if (ptr->d_type == 8)
        {
            std::string tmp = basePath + "/" + std::string(ptr->d_name);
            files.push_back(tmp);
        }
    }

}

int getFileLastMod(const std::string &filename)
{
    struct stat sbuf;
    if (stat(filename.c_str(), &sbuf) < 0)
    {
        std::cout << "can't find this file\n";
        return 1;
    }
    if (!S_ISREG(sbuf.st_mode))
        return 1;

    formatTime time;
    time.fmt = "%a, %d %b %Y %H:%M:%S %Z";
    time_t src = sbuf.st_mtim.tv_sec;
    struct tm gmt;

    convertTime2Gmt(&src, true, &gmt);
    formate(&gmt, &time);
    std::cout << time.str << "++" << src << std::endl;
}

void getAllFileTime(const std::string &basePath)
{
    std::vector<std::string> files;
    readFileList(basePath, files);
    for (auto f:files)
        getFileLastMod(f);
}

int main_()
{
    //testPraseHttpTime();

    std::string path;
    std::cin >> path;
    getAllFileTime(path);
    return 0;
}

int main()
{
    testPraseHttpTime();
}
