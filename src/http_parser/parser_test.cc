/*
 *Author:GeneralSandman
 *Code:https://github.com/GeneralSandman/TinyWeb
 *E-mail:generalsandman@163.com
 *Web:www.generalsandman.cn
 */

/*---XXX---
 *
 ****************************************
 *
 */

#include "my_http_parser.h"

#include <iostream>
#include <vector>
#include <string>

using namespace std;





void testMyHttpParser()
{
    vector<string> urls;
    //  = {
    //     "http://www.dissigil.cn",
    //     "http://www.baidu.com",
    //     "https://www.github.com"};

    string request = "";

    HttpParser parser;

    for (auto url : urls)
    {
        int index = 0;
        parser.execute(request, index);
    }
}
int main()
{
    std::cout << "main function\n";
    return 0;
}

int functionBeforeMain()
{
    std::cout << "invoke functionBeforeMain()\n";
    main();
}
