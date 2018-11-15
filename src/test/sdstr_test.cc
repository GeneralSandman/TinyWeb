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

#include <tiny_base/memorypool.h>
#include <tiny_struct/sdstr_t.h>

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdarg.h>
using namespace std;

int test1()
{
    return 0;
}

int test2()
{

    std::string str = "helloworld";
    void *res1 = malloc(10);
    memcpy(res1, (void *)(str.c_str()), str.size());
    char *tmp = (char *)res1;
    printf("%.*s\n", str.size(), tmp);

    void *res2 = realloc(res1, 20);
    tmp = (char*)res2;
    memcpy(res2+10, (void *)(str.c_str()), str.size());
    printf("%.*s\n", str.size() * 2, tmp);

    free(res2);

    return 0;

}

int test3()
{
    std::string str = "helloworld";
    unsigned int len = str.size();

    {
        sdstr tmp;
        sdsnset(&tmp, nullptr, 0);
        sdsncat(&tmp, str.c_str(), len);
        printf(&tmp);
        destory(&tmp);
    }

    {
        sdstr tmp;
        sdsnset(&tmp, nullptr, 0);
        sdscat(&tmp, str.c_str());
        printf(&tmp);
        destory(&tmp);
    }

    {
        sdstr tmp;
        sdsnset(&tmp, nullptr, 0);

        sdstr tmp2;
        std::string str2 = "zhl";
        sdsnset(&tmp2, str2.c_str(), str2.size());

        sdsncatsds(&tmp, &tmp2, tmp2.len);
        printf(&tmp);

        destory(&tmp);
        destory(&tmp2);
    }

    {
        sdstr tmp;
        sdsnset(&tmp, str.c_str(), str.size());

        std::string s = "zhenhuli";
        sdsncpy(&tmp, s.c_str(), s.size());
        printf(&tmp);

        destory(&tmp);
    }

    {
        sdstr tmp;
        sdsnset(&tmp, str.c_str(), str.size());

        std::string s = "zhenhuli";
        sdscpy(&tmp, s.c_str());
        printf(&tmp);

        destory(&tmp);

    }

    {
        sdstr tmp1;
        sdstr tmp2;

        std::string s = "zhenhuli";
        sdsnset(&tmp1, str.c_str(), str.size());
        sdsnset(&tmp2, s.c_str(), s.size());

        printf(&tmp1);
        printf(&tmp2);

        sdsncpysds(&tmp2, &tmp1, tmp1.len);
        printf(&tmp2);

        destory(&tmp1);
        destory(&tmp2);
    }

    {
        sdstr tmp1;
        sdstr tmp2;

        std::string s = "zhenhuli";
        sdsnset(&tmp1, str.c_str(), str.size());
        sdsnset(&tmp2, s.c_str(), s.size());

        printf(&tmp1);
        printf(&tmp2);

        sdsncpysds(&tmp2, &tmp1, tmp1.len);
        printf(&tmp2);

        destory(&tmp1);
    }

    {

    }
    return 0;
}

void test4()
{
    {
        sdstr tmp;
        sdssetull(&tmp, 0);
        printf(&tmp);
        destory(&tmp);
    }
    {
        sdstr tmp;
        sdssetull(&tmp, 10);
        printf(&tmp);
        destory(&tmp);
    }
    {
        sdstr tmp;
        sdssetull(&tmp, 100);
        printf(&tmp);
        destory(&tmp);
    }
    {
        sdstr tmp;
        sdssetull(&tmp, 1000);
        printf(&tmp);
        destory(&tmp);
    }
    {
        sdstr tmp;
        sdssetull(&tmp, 10000);
        printf(&tmp);
        destory(&tmp);
    }
    {
        sdstr tmp;
        sdssetull(&tmp, 100000);
        printf(&tmp);
        destory(&tmp);
    }
    {
        sdstr tmp;
        sdssetull(&tmp, 1000000);
        printf(&tmp);
        destory(&tmp);
    }

    //------

    {
        sdstr tmp;
        sdssetll(&tmp, 0);
        printf(&tmp);
        destory(&tmp);
    }
    {
        sdstr tmp;
        sdssetll(&tmp, 10);
        printf(&tmp);
        destory(&tmp);
    }
    {
        sdstr tmp;
        sdssetll(&tmp, 100);
        printf(&tmp);
        destory(&tmp);
    }
    {
        sdstr tmp;
        sdssetll(&tmp, 1000);
        printf(&tmp);
        destory(&tmp);
    }
    {
        sdstr tmp;
        sdssetll(&tmp, 10000);
        printf(&tmp);
        destory(&tmp);
    }
    {
        sdstr tmp;
        sdssetll(&tmp, 100000);
        printf(&tmp);
        destory(&tmp);
    }
    {
        sdstr tmp;
        sdssetll(&tmp, 1000000);
        printf(&tmp);
        destory(&tmp);
    }

    //------
    {
        sdstr tmp;
        sdssetll(&tmp, -0);
        printf(&tmp);
        destory(&tmp);
    }
    {
        sdstr tmp;
        sdssetll(&tmp, -10);
        printf(&tmp);
        destory(&tmp);
    }
    {
        sdstr tmp;
        sdssetll(&tmp, -100);
        printf(&tmp);
        destory(&tmp);
    }
    {
        sdstr tmp;
        sdssetll(&tmp, -1000);
        printf(&tmp);
        destory(&tmp);
    }
    {
        sdstr tmp;
        sdssetll(&tmp, -10000);
        printf(&tmp);
        destory(&tmp);
    }
    {
        sdstr tmp;
        sdssetll(&tmp, -100000);
        printf(&tmp);
        destory(&tmp);
    }
    {
        sdstr tmp;
        sdssetll(&tmp, -1000000);
        printf(&tmp);
        destory(&tmp);
    }

}

int my_snprintf(char *s, int size, const char *fmt, ...) //该自定义函数，与系统提供的snprintf()函数相同。
{
    std::cout << "fmt size(" << strlen(fmt) << ")\n";
    va_list ap;
    int n=0;
    va_start(ap, fmt); //获得可变参数列表
    n=vsnprintf (s, size, fmt, ap); //写入字符串s
    va_end(ap); //释放资源
    return n; //返回写入的字符个数
}

void test5()
{
    {
        std::string str;
        for(int i=0;i<2024;i++)
            str += char('0'+i%10);
        std::cout << str.size() << std::endl;
        sdstr tmp;
        sdssprintf(&tmp,"a=%d,b=%d,s=%s",101,101,str.c_str());
        printf(&tmp);
        destory(&tmp);
    }
}

int main()
{
    test5();
}
