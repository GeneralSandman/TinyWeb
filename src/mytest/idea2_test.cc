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

#include <tiny_http/http_parser.h>

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>

typedef struct range {
    const char* str;
    bool valid;
    int type_num;
};

range headers[] = {
    {
        .str = "bytes=0-100",
        .valid = true,
        .type_num = 8,
    },

    {
        .str = "bytes=0-100,200-300",
        .valid = true,
        .type_num = 8,
    },

    {
        .str = "bytes=5-",
        .valid = true,
        .type_num = 8,
    },

    {
        .str = "bytes=-5",
        .valid = true,
        .type_num = 8,
    },

    {
        .str = "bytes=0-100,-300",
        .valid = true,
        .type_num = 8,
    },

    {
        .str = "bytes=0-100,200-",
        .valid = true,
        .type_num = 8,
    },

    {
        .str = "bytes=0-,200-300",
        .valid = true,
        .type_num = 8,
    },

    {
        .str = " bytes= -100, 200-300",
        .valid = true,
        .type_num = 8,
    },

    {
        .str = "bytes=-100, 200-300",
        .valid = true,
        .type_num = 8,
    },

    {
        .str = "bytes=-100,200-300",
        .valid = true,
        .type_num = 8,
    },

    {
        .str = "bytes = -100,200-300",
        .valid = true,
        .type_num = 8,
    },

    {
        .str = "bytes=- 100,200 - 300",
        .valid = false,
        .type_num = 8,
    },

    {
        .str = "bytes = -100,200 - 300",
        .valid = false,
        .type_num = 8,
    },

};

using namespace std;

int parseUnit(const char* str, unsigned int len)
{
    if (0 == strncmp("bytes", str, 5)) {
        return 0;
    }
    return -1;
}

int parseRange(const char* str, unsigned int len)
{
    unsigned int i;
    char ch;
    for (i = 0; i < len; i++) {
        ch = *(str + i);
    }

    printf("len:(%d),data:(%s),\n", len, str);
    return 0;
}

static unsigned int static_num = 0;

int parseHeaderRange(const char* str, unsigned int len)
{
    char* post;
    char delim[] = "=,";
    char new_str[len + 1];
    int return_val;
    bool begin = true;

    strcpy(new_str, str);
    new_str[len] = '\0';

    post = strtok(new_str, delim);
    while (post != NULL) {
        if (begin) {
            return_val = parseUnit(post, strlen(post));
            begin = false;
        } else {
            return_val = parseRange(post, strlen(post));
        }
        if (return_val == -1) {
            return -1;
        }
        static_num++;

        post = strtok(NULL, delim);
    }
    return 0;
}

void parseHeaderRange_test()
{
    vector<int> notPass;
    int alltest = 0;
    int passtest = 0;

    int len = sizeof(headers) / sizeof(headers[0]);

    for (int i = 0; i < len; i++) {
        std::cout << i << ")" << std::endl;
        alltest++;

        static_num = 0;
        int tmp = parseHeaderRange(headers[i].str, strlen(headers[i].str));
        bool res = (tmp == -1) ? false : true;
        if (res == headers[i].valid) {
            passtest++;

            // if (res) {
            //
            // if (headers[i].type_num == static_num) {
            // passtest++;
            // } else {
            // notPass.push_back(i);
            // }
            // } else {
            // passtest++;
            // }
        } else {
            notPass.push_back(i);
        }
    }

    std::cout << "[Parse Range-Header Test] pass/all = " << passtest << "/" << alltest << std::endl;

    if (!notPass.empty()) {
        std::cout << "not header index:\t";
        for (auto t : notPass)
            std::cout << t << " ";
        std::cout << std::endl;
    }
}

int main()
{
    parseHeaderRange_test();

    return 0;
}
