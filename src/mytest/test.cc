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


#include <stdio.h> 
#include <string.h> 
#include <iostream> 

int main()
{
    char str[] = "0000000000000000+\r\n";
    
    unsigned int size = 65535;
    std::cout << sprintf(str, "%x\r\n", size) << std::endl;
    

    size_t len = strlen(str);
    printf("size:%u,data:%.*s\n", len, len, str);

    return 0;
}

