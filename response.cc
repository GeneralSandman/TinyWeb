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

#include "response.h"
#include "http.h"
#include "log.h"

HttpResponse::HttpResponse()
{
    LOG(Debug) << "class HttpResponse constructor\n";
}

HttpResponse::~HttpResponse()
{
    LOG(Debug) << "class HttpResponse destructor\n";
}
