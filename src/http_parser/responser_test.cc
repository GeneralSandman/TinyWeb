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

#include"http.h"
#include"http_responser.h"

#include <iostream>
#include <fstream>

using namespace std;

int main()
{
    for(int i=0; i<61; i++)
    {
        std::string result = special_response_body((enum http_status)i);
        std::cout << result << std::endl;

        std::string fileName = httpStatusCode((enum http_status)i);
        if(fileName == "<invalid>")
            continue;
        fileName = "www/" + fileName + ".html";
        ofstream fout(fileName.c_str());
        if(fout)
        {
            fout << result << std::endl;
            fout.close();
        }

    }

    return 0;
}


