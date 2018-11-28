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

#include <tiny_http/http_model_file.h>

#include <iostream>

using namespace std;

void testFile()
{
    std::cout << "input file =";
    std::string fname;
    std::cin >> fname;

    File inputFile;
    
    int return_val = initFile(&inputFile, fname);
    if(return_val < 0)
    {
        std::cout << return_val << std::endl;
        return ;
    }

    sendfile(0, &inputFile);

    destoryFile(&inputFile);
}

void testFile2()
{

    std::cout << "input file =";
    std::string fname;
    std::cin >> fname;

    File inputFile;
    
    int return_val = initFile(&inputFile, fname);
    if(return_val < 0)
    {
        std::cout << return_val << std::endl;
        return ;
    }

    time_t t = inputFile.info.st_mtime;
    char tmpBuf[156];
    strftime(tmpBuf, 156, "%Y-%m-%d %H:%M:%S", localtime(&t)); //format date and time.

    std::cout << "last modified time:" << tmpBuf << std::endl;
    sendfile(2, &inputFile);

    destoryFile(&inputFile);

}

int main()
{
    testFile();
    // testFile2();

    return 0;
}
