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

void test1()
{
    std::string str = "zhenhuli";
    unsigned int len = str.size();

    {
        sdstr tmp;
        sdsnnew(&tmp, nullptr, 0);
        printf(&tmp);
        destory(&tmp);
    }

    {
        sdstr tmp;
        sdsnew(&tmp, nullptr);
        printf(&tmp);
        destory(&tmp);
    }

    {
        sdstr tmp;
        sdsnewempty(&tmp);
        printf(&tmp);
        destory(&tmp);
    }

    {
        sdstr tmp;
        sdsnnew(&tmp, str.c_str(), len);
        printf(&tmp);
        destory(&tmp);
    }

    {
        sdstr tmp;
        sdsnew(&tmp, str.c_str());
        printf(&tmp);
        destory(&tmp);
    }

    {
        sdstr tmp;
        sdsnew(&tmp, str.c_str());
        printf(&tmp);
        destory(&tmp);
    }

    {
        sdstr tmp;

        {
            sdstr s1;
            sdsnew(&s1, str.c_str());
            sdsnewdup(&tmp, &s1);
            destory(&s1);
        }

        printf(&tmp);
        destory(&tmp);
    }
}

void test2()
{
    std::string str1 = "zhenhuli";
    unsigned int len1 = str1.size();

    std::string str2 = "helloworld";
    unsigned int len2 = str2.size();

    sdstr sds;
    sdsnew(&sds, str1.c_str());

    sdstr sds2;
    sdsnew(&sds2, str2.c_str());

    {
        sdstr tmp;
        sdsnewempty(&tmp);
        sdsncat(&tmp, str1.c_str(), len1);
        printf(&tmp);

        sdsncat(&tmp, str2.c_str(), len2);
        printf(&tmp);

        destory(&tmp);
    }

    {
        sdstr tmp;
        sdsnewempty(&tmp);
        sdscat(&tmp, str1.c_str());
        printf(&tmp);

        sdscat(&tmp, str2.c_str());
        printf(&tmp);

        destory(&tmp);
    }

    {
        sdstr tmp;
        sdsnewempty(&tmp);
        sdsncatsds(&tmp, &sds, sds.len);
        printf(&tmp);

        sdsncatsds(&tmp, &sds2, sds2.len);
        printf(&tmp);

        destory(&tmp);
    }

    {
        sdstr tmp;
        sdsnewempty(&tmp);
        sdscatsds(&tmp, &sds);
        printf(&tmp);

        sdscatsds(&tmp, &sds2);
        printf(&tmp);

        destory(&tmp);
    }

    destory(&sds);
    destory(&sds2);

}

void test3()
{
    std::string str1 = "zhenhuli";
    unsigned int len1 = str1.size();

    std::string str2 = "helloworld";
    unsigned int len2 = str2.size();

    sdstr sds;
    sdsnew(&sds, str1.c_str());

    sdstr sds2;
    sdsnew(&sds2, str2.c_str());

    {
        sdstr tmp;
        sdsnew(&tmp, str1.c_str());
        printf(&tmp);
        sdsncpy(&tmp, str2.c_str(), len2);
        printf(&tmp);
        destory(&tmp);
    }
    {
        sdstr tmp;
        sdsnew(&tmp, str1.c_str());
        printf(&tmp);
        sdscpy(&tmp, str2.c_str());
        printf(&tmp);
        destory(&tmp);
    }
    {
        sdstr tmp;
        sdsnew(&tmp, str1.c_str());
        printf(&tmp);
        sdsncpysds(&tmp, &sds2, sds2.len);
        printf(&tmp);
        destory(&tmp);
    }
    {
        sdstr tmp;
        sdsnew(&tmp, str1.c_str());
        printf(&tmp);
        sdscpysds(&tmp, &sds2);
        printf(&tmp);
        destory(&tmp);
    }

    std::cout << "--------\n";

    {
        sdstr tmp;
        sdsnew(&tmp, str2.c_str());
        printf(&tmp);
        sdsncpy(&tmp, str1.c_str(), len1);
        printf(&tmp);
        destory(&tmp);
    }
    {
        sdstr tmp;
        sdsnew(&tmp, str2.c_str());
        printf(&tmp);
        sdscpy(&tmp, str1.c_str());
        printf(&tmp);
        destory(&tmp);
    }
    {
        sdstr tmp;
        sdsnew(&tmp, str2.c_str());
        printf(&tmp);
        sdsncpysds(&tmp, &sds, sds.len);
        printf(&tmp);
        destory(&tmp);
    }
    {
        sdstr tmp;
        sdsnew(&tmp, str2.c_str());
        printf(&tmp);
        sdscpysds(&tmp, &sds);
        printf(&tmp);
        destory(&tmp);
    }

    std::cout << "--------\n";

    destory(&sds);
    destory(&sds2);
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

void test5()
{
    {
        std::string str;
        for(int i=0;i<2024;i++)
            str += char('0'+i%10);
        std::cout << str.size() << std::endl;
        sdstr tmp;
        sdsnewempty(&tmp);
        sdscatsprintf(&tmp,"a=%d,b=%d,s=%s",101,101,str.c_str());
        printf(&tmp);
        destory(&tmp);
    }
}

void test6()
{
    {
        sdstr tmp;
        sdsnewempty(&tmp);

        for (int i=0; i<1024; i++)
        {
            sdsncat(&tmp, "1", 1);
            printf(&tmp);
        }

        destory(&tmp);
    }
    std::cout << "----------\n";
    {
        sdstr tmp;
        sdsnewempty(&tmp, 1024);

        for (int i=0; i<1024; i++)
        {
            sdsncat(&tmp, "1", 1);
            printf(&tmp);
        }

        destory(&tmp);
    }
}

void test7()
{
    std::string str = "helloworld";
    unsigned int len = str.size();

    std::string sep = ".";
    unsigned int seplen = sep.size();

    sdstr sdss[100];
    for (int i=0; i<100; i++)
    {
        sdsnew(sdss+i, str.c_str());
    }


    sdstr result;
    sdsnewempty(&result);
    sdsjoinsds(&result, sdss, 100, sep.c_str(), seplen);
    printf(&result);
    destory(&result);


    for (int i=0; i<100; i++)
    {
        destory(sdss+i);
    }
}

void test8()
{
    std::string str = "helloworld";
    unsigned int len = str.size();

    const char * sdss[100];
    for (int i=0; i<100; i++)
    {
        *(sdss+i) = str.c_str();
    }

    std::string sep = ".";
    unsigned int seplen = sep.size();


    sdstr result;
    sdsnewempty(&result);
    sdsjoinstr(&result, sdss, 100, sep.c_str(), seplen);
    printf(&result);
    destory(&result);

}

void errtest()
{
    {
        sdstr tmp;
        std::string str = "helloworld";
        unsigned int len = str.size();
        sdsnnew(&tmp, str.c_str(), len);
        printf(&tmp);
        destory(&tmp);
    }
    {
        sdstr tmp;
        const char *str = "helloworld";
        sdsnnew(&tmp, str, strlen(str));
        printf(&tmp);
        destory(&tmp);
    }
    {
        sdstr tmp;
        char *str = "helloworld";
        sdsnnew(&tmp, str, strlen(str));
        printf(&tmp);
        destory(&tmp);
    }
    {
        sdstr tmp;
        char str[] = "helloworld";
        sdsnnew(&tmp, str, strlen(str));
        printf(&tmp);
        destory(&tmp);
    }
    {
        sdstr tmp;
        sdsnnew(&tmp, "helloworld", 10);
        printf(&tmp);
        destory(&tmp);
    }
}

int main(int argc, char *argv[])
{
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    test7();
    test8();
    errtest();
}
