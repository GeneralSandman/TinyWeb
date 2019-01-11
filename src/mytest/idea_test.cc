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

void test1()
{
    char str[] = "0000000000000000+\r\n";

    unsigned int size = 65535;
    std::cout << sprintf(str, "%x\r\n", size) << std::endl;

    size_t len = strlen(str);
    printf("size:%u,data:%.*s\n", len, len, str);
}

typedef struct accept {
    const char* str;
    bool valid;
    int type_num;
};

accept headers[] = {
    {
        .str = "text/html, "
               "application/xhtml+xml, "
               "application/xml, image/webp,*/*,"
               "text/html;level=1, "
               "*/*,application/xml;q=0.8,",
        .valid = true,
        .type_num = 8,
    },

    {
        .str = "text/html",
        .valid = true,
        .type_num = 1,
    },

    {
        .str = "text/html,"
               "application/xhtml+xml",
        .valid = true,
        .type_num = 2,
    },

    {
        .str = "text/html,"
               "application/xhtml+xml,"
               "application/xml",
        .valid = true,
        .type_num = 3,
    },

    {
        .str = "text/html,"
               "application/xhtml+xml,"
               "application/xml,"
               "image/webp",
        .valid = true,
        .type_num = 4,
    },

    {
        .str = "text/html,"
               "application/xhtml+xml,"
               "application/xml,"
               "image/webp,"
               "image/apng",
        .valid = true,
        .type_num = 5,
    },

    {
        .str = "text/html,"
               "application/xhtml+xml,"
               "application/xml,"
               "image/webp,"
               "image/apng,"
               "application/signed-exchange",
        .valid = true,
        .type_num = 6,
    },

    {
        .str = "text/html,",
        .valid = true,
        .type_num = 1,
    },

    {
        .str = "text/html,"
               "application/xhtml+xml,",
        .valid = true,
        .type_num = 2,
    },

    {
        .str = "text/html,"
               "application/xhtml+xml,"
               "application/xml,",
        .valid = true,
        .type_num = 3,
    },

    {
        .str = "text/html,"
               "application/xhtml+xml,"
               "application/xml,"
               "image/webp,",
        .valid = true,
        .type_num = 4,
    },

    {
        .str = "text/html,"
               "application/xhtml+xml,"
               "application/xml,"
               "image/webp,"
               "image/apng,",
        .valid = true,
        .type_num = 5,
    },

    {
        .str = "text/html,"
               "application/xhtml+xml,"
               "application/xml,"
               "image/webp,"
               "image/apng,"
               "application/signed-exchange,",
        .valid = true,
        .type_num = 6,
    },

    {
        .str = "text/html , ",
        .valid = true,
        .type_num = 1,
    },

    {
        .str = "text/html , "
               "application/xhtml+xml , ",
        .valid = true,
        .type_num = 2,
    },

    {
        .str = "text/html , "
               "application/xhtml+xml, "
               "application/xml ,",
        .valid = true,
        .type_num = 3,
    },

    {
        .str = "text/html,"
               "application/xhtml+xml,"
               "application/xml ,"
               "image/webp,",
        .valid = true,
        .type_num = 4,
    },

    {
        .str = "text/html,"
               "application/xhtml+xml ,"
               "application/xml,"
               "image/webp, "
               "image/apng,",
        .valid = true,
        .type_num = 5,
    },

    {
        .str = "text/html,"
               "application/xhtml+xml,"
               "application/xml,"
               "image/webp,"
               "image/apng,"
               "application/signed-exchange, ",
        .valid = true,
        .type_num = 6,
    },

    {
        .str = "text/html,"
               "application/xhtml+xml,"
               "application/xml,"
               "image/webp,"
               "*/*,"
               "application/signed-exchange, ",
        .valid = true,
        .type_num = 6,
    },

    {
        .str = "text/html;q=0.9,"
               "application/xhtml+xml,"
               "application/xml;q=0.9,"
               "image/webp,"
               "image/apng;q=0.9,"
               "application/signed-exchange, ",
        .valid = true,
        .type_num = 6,
    },

    {
        .str = "text/html;q=0.9;v=b3,"
               "application/xhtml+xml;v=b3,"
               "application/xml;q=0.9,"
               "image/webp,"
               "image/apng;q=0.9;v=b3,"
               "application/signed-exchange, ",
        .valid = true,
        .type_num = 6,
    },

    {
        .str = "text/html;level=1;v=b3,"
               "application/xhtml+xml;v=b3,"
               "application/xml;level=1,"
               "image/webp,"
               "image/apng;q=0.9;v=b3,"
               "application/signed-exchange, ",
        .valid = true,
        .type_num = 6,
    },

    {
        .str = "text/html;format=flowed;v=b3,"
               "application/xhtml+xml;v=b3,"
               "application/xml;format=flowed,"
               "image/webp,"
               "image/apng;q=0.9;v=b3,"
               "application/signed-exchange;format=flowed, ",
        .valid = true,
        .type_num = 6,
    },

};

using namespace std;

int parseMimeType(const char* str, unsigned int len)
{
    unsigned int i;
    char ch;
    for (i = 0; i < len; i++) {
        ch = *(str + i);

        if (ch == ';') {
            break;
        }

        if (!isMimeTypeChar(ch)) {
            printf("[Debug] error: mimetype char(%c) is valid\n", ch);
            return -1;
        }
    }

    printf("len:(%d),data:(%s),mime-type(%.*s)\n", len, str, i, str);
    return 0;
}

static unsigned int static_num = 0;

int parseHeaderAccept(const char* str, unsigned int len)
{
    char* post;
    char delim[] = " ,";
    char new_str[len + 1];
    int return_val;

    strcpy(new_str, str);
    new_str[len] = '\0';

    post = strtok(new_str, delim);
    while (post != NULL) {
        return_val = parseMimeType(post, strlen(post));
        if (return_val == -1) {
            return -1;
        }
        static_num++;

        post = strtok(NULL, delim);
    }
    return 0;
}

void parseHeaderAccept_test()
{
    vector<int> notPass;
    int alltest = 0;
    int passtest = 0;

    int len = sizeof(headers) / sizeof(headers[0]);

    for (int i = 0; i < len; i++) {
        std::cout << i << ")" << std::endl;
        alltest++;

        static_num = 0;
        int tmp = parseHeaderAccept(headers[i].str, strlen(headers[i].str));
        bool res = (tmp == -1) ? false : true;
        if (res == headers[i].valid) {

            if (res) {

                if (headers[i].type_num == static_num) {
                    passtest++;
                } else {
                    notPass.push_back(i);
                }
            } else {
                passtest++;
            }
        } else {
            notPass.push_back(i);
        }
    }

    std::cout << "[Parse Accept-Header Test] pass/all = " << passtest << "/" << alltest << std::endl;

    if (!notPass.empty()) {
        std::cout << "not header index:\t";
        for (auto t : notPass)
            std::cout << t << " ";
        std::cout << std::endl;
    }
}

int main()
{
    parseHeaderAccept_test();

    return 0;
}
