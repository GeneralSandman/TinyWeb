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

#include "http_model_file.h"

#include <iostream>

using namespace std;

int main()
{
    
    std::cout << "input file =";
    std::string fname;
    std::cin >> fname;

    File inputFile;
    
    int return_val = initFile(&inputFile, fname);
    if(return_val < 0)
    {
        std::cout << return_val << std::endl;
        return 1;
    }

    sendfile(0, &inputFile);

    destoryFile(&inputFile);

    return 0;
}
