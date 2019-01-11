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

#include <tiny_http/http_time.h>
#include <tiny_struct/sdstr_t.h>

#include <dirent.h>
#include <iostream>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <vector>

#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

using namespace std;

typedef struct testTime {
    time_t time;
    bool valid;
    const char* httpTimeStr;
    const char* cookieTimeStr;
    unsigned int timestamp;
} testTime;

testTime times[] = {
    // formate 1
    {
        .time = 1541680751,
        .valid = true,
        .httpTimeStr = "Thu, 08 Nov 2018 12:39:11 GMT",
        .cookieTimeStr = "Thursday, 08-Nov-18 12:39:11 GMT",
        .timestamp = 0,
    },

    {
        .time = 1541680751,
        .valid = true,
        .httpTimeStr = "Thu,08Nov201812:39:11GMT",
        .cookieTimeStr = "Thursday,08-Nov-1812:39:11GMT",
        .timestamp = 0,
    },

    {
        .time = 1541680705,
        .valid = true,
        .httpTimeStr = "Thu, 08 Nov 2018 12:38:25 GMT",
        .cookieTimeStr = "Thursday, 08-Nov-18 12:38:25 GMT",
        .timestamp = 0,
    },

    {
        .time = 1541680705,
        .valid = true,
        .httpTimeStr = "Thu,08Nov201812:38:25GMT",
        .cookieTimeStr = "Thursday, 08-Nov-1812:38:25 GMT",
        .timestamp = 0,
    },

    {
        .time = 1541574037,
        .valid = true,
        .httpTimeStr = "Wed, 07 Nov 2018 07:00:37 GMT",
        .cookieTimeStr = "Wednesday, 07-Nov-18 07:00:37 GMT",
        .timestamp = 0,
    },

    {
        .time = 1541574037,
        .valid = true,
        .httpTimeStr = "Wed,07 Nov 201807:00:37 GMT",
        .cookieTimeStr = "Wednesday,07-Nov-18 07:00:37 GMT",
        .timestamp = 0,
    },

    {
        .time = 1541680684,
        .valid = true,
        .httpTimeStr = "Thu, 08 Nov 2018 12:38:04 GMT",
        .cookieTimeStr = "Thursday, 08-Nov-18 12:38:04 GMT",
        .timestamp = 0,
    },

    {
        .time = 1541680684,
        .valid = true,
        .httpTimeStr = "Thu, 08Nov 2018 12:38:04 GMT",
        .cookieTimeStr = "Thursday, 08-Nov-1812:38:04 GMT",
        .timestamp = 0,
    },

    {
        .time = 1542444072,
        .valid = true,
        .httpTimeStr = "Sat, 17 Nov 2018 08:41:12 GMT",
        .cookieTimeStr = "Saturday, 17-Nov-18 08:41:12 GMT",
        .timestamp = 0,
    },

    {
        .time = 1542444072,
        .valid = true,
        .httpTimeStr = "Sat,17 Nov 2018 08:41:12GMT",
        .cookieTimeStr = "Saturday, 17-Nov-18 08:41:12GMT",
        .timestamp = 0,
    },

    {
        .time = 1542383480,
        .valid = true,
        .httpTimeStr = "Fri, 16 Nov 2018 15:51:20 GMT",
        .cookieTimeStr = "Friday, 16-Nov-18 15:51:20 GMT",
        .timestamp = 0,
    },

    {
        .time = 1542383480,
        .valid = true,
        .httpTimeStr = "Fri,16 Nov2018 15:51:20GMT",
        .cookieTimeStr = "Friday, 16-Nov-1815:51:20 GMT",
        .timestamp = 0,
    },

    {
        .time = 1542390346,
        .valid = true,
        .httpTimeStr = "Fri, 16 Nov 2018 17:45:46 GMT",
        .cookieTimeStr = "Friday, 16-Nov-18 17:45:46 GMT",
        .timestamp = 0,
    },

    {
        .time = 1542390346,
        .valid = true,
        .httpTimeStr = "Fri, 16 Nov2018 17:45:46 GMT",
        .cookieTimeStr = "Friday,16-Nov-18 17:45:46 GMT",
        .timestamp = 0,
    },

    {
        .time = 1541680751,
        .valid = true,
        .httpTimeStr = "Thu, 08 Nov 2018 12:39:11 GMT",
        .cookieTimeStr = "Thursday, 08-Nov-18 12:39:11 GMT",
        .timestamp = 0,
    },

    {
        .time = 1541680751,
        .valid = true,
        .httpTimeStr = "Thu, 08 Nov2018 12:39:11 GMT",
        .cookieTimeStr = "Thursday,08-Nov-18 12:39:11 GMT",
        .timestamp = 0,
    },
};

bool testParseHttpTime1(testTime* t)
{
    sdstr tmp;
    sdsnewempty(&tmp, 32);

    // Test http formate.
    formatHttpTime(t->time, &tmp);
    bool sameHttpTime = false;
    if (0 == strncmp(t->httpTimeStr, tmp.data, tmp.len)) {
        printf("1same\n");
        sameHttpTime = true;
    } else {
        printf("1error:<%.*s><%.*s>\n", strlen(t->httpTimeStr), t->httpTimeStr, tmp.len, tmp.data);
    }

    // Test deformate.
    time_t time_tmp;
    deformatHttpTime(&tmp, &time_tmp);
    bool sameTimet = false;
    if (time_tmp == t->time) {
        printf("2same\n");
        sameTimet = true;
    } else {
        printf("2error:%lu-%lu\n", t->time, time_tmp);
    }

    sdsdestory(&tmp);
    return sameHttpTime && sameTimet;
}

bool testParseHttpTime2(testTime* t)
{
    // Test deformate.
    sdstr str1;
    sdsnnew(&str1, t->httpTimeStr, strlen(t->httpTimeStr));

    time_t time_tmp;
    deformatHttpTime(&str1, &time_tmp);
    bool sameTimet = false;
    if (time_tmp == t->time) {
        printf("3same\n");
        sameTimet = true;
    } else {
        printf("3error:%lu-%lu\n", t->time, time_tmp);
    }

    // Test http formate.
    sdstr str2;
    sdsnewempty(&str2, 32);

    formatHttpTime(time_tmp, &str2);
    bool sameHttpTime = false;
    if (0 == strncmp(t->httpTimeStr, str2.data, str1.len)) {
        printf("4same\n");
        sameTimet = true;
    } else {
        printf("4error:<%.*s><%.*s>\n", strlen(t->httpTimeStr), t->httpTimeStr, str2.len, str2.data);
    }

    sdsdestory(&str1);
    sdsdestory(&str2);
    return sameHttpTime && sameTimet;
}

bool testParseCookieTime1(testTime* t)
{
    sdstr tmp;
    sdsnewempty(&tmp, 32);

    // Test cookie formate.
    formatCookieTime(t->time, &tmp);
    bool sameCookieTime = false;
    if (0 == strncmp(t->cookieTimeStr, tmp.data, tmp.len))
        sameCookieTime = true;
    else {
        printf("5error:<%.*s><%.*s>\n", strlen(t->httpTimeStr), t->httpTimeStr, tmp.len, tmp.data);
    }

    // Test deformate.
    time_t time_tmp;
    deformatCookieTime(&tmp, &time_tmp);
    bool sameTimet = false;
    if (time_tmp == t->time)
        sameTimet = true;
    else {
        printf("6error:%lu-%lu\n", t->time, time_tmp);
    }

    destory(&tmp);
    return sameCookieTime && sameTimet;
}

bool testParseCookieTime2(testTime* t)
{
    // Test deformate.
    sdstr str1;
    sdsnnew(&str1, t->cookieTimeStr, strlen(t->cookieTimeStr));

    time_t time_tmp;
    deformatCookieTime(&str1, &time_tmp);
    bool sameTimet = false;
    if (time_tmp == t->time) {
        sameTimet = true;
        printf("7same\n");
    } else {
        printf("7error:%lu-%lu\n", t->time, time_tmp);
    }

    // Test http formate.
    sdstr str2;
    sdsnewempty(&str2, 32);

    formatCookieTime(time_tmp, &str2);
    bool sameCookieTime = false;
    if (0 == strncmp(t->cookieTimeStr, str2.data, str1.len)) {
        sameCookieTime = true;
        printf("8same\n");
    } else {
        printf("8error:<%.*s><%.*s>\n", strlen(t->cookieTimeStr), t->cookieTimeStr, str2.len, str2.data);
    }

    sdsdestory(&str1);
    sdsdestory(&str2);
    return sameCookieTime && sameTimet;
}

void testPraseTime()
{
    int alltest = 0;
    int passtest = 0;

    int len = sizeof(times) / sizeof(times[0]);

    for (int i = 0; i < len; i++) {
        std::cout << i << ")" << std::endl;
        alltest++;

        // bool a = testParseHttpTime1(times + i);
        bool b = testParseHttpTime2(times + i);
        // bool c = testParseCookieTime1(times + i);
        bool d = testParseCookieTime2(times + i);
        // bool res = a && b && c && d;
        bool res = b && d;
        if (res)
            passtest++;
        else
            std::cout << "no pass test" << std::endl;
    }

    std::cout << "[Parse HttpTime Test] pass/all = " << passtest << "/" << alltest << std::endl;
}

int readFileList(const std::string& basePath, std::vector<std::string>& files)
{
    DIR* dir;
    struct dirent* ptr;

    if ((dir = opendir(basePath.c_str())) == nullptr) {
        printf("open dir(%s) error\n", basePath.c_str());
        return 1;
    }

    while ((ptr = readdir(dir)) != nullptr) {
        if (ptr->d_type == 8) {
            std::string tmp = basePath + "/" + std::string(ptr->d_name);
            files.push_back(tmp);
        }
    }
}

int getFileLastMod(const std::string& filename)
{
    struct stat sbuf;
    if (stat(filename.c_str(), &sbuf) < 0) {
        std::cout << "can't find this file\n";
        return 1;
    }
    if (!S_ISREG(sbuf.st_mode))
        return 1;
}

void getAllFileTime(const std::string& basePath)
{
    std::vector<std::string> files;
    readFileList(basePath, files);
    for (auto f : files)
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
    testPraseTime();
}
