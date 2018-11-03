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

#ifndef HTTP_RESPONSER_H
#define HTTP_REsPONSER_H

#include"http.h"

#include<iostream>

std::string special_response_body(enum http_status);

class HttpResponser
{

public:
    HttpResponser()
    {
        std::cout << "class HttpResponser constructor\n";
    }
    
    int writeResponseLine()
    {

    }

    int writeHeader()
    {

    }

    int writeBody()
    {

    }

    int writeResponse()
    {
        writeResponseLine();
        writeHeader();
        writeBody();
    }

    ~HttpResponser()
    {
        std::cout << "class HttpResponser destructor\n";
    }
};

#endif
