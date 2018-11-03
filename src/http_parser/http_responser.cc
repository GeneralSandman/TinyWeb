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

#include<iostream>
#include<string>


#define TINYWEB_VERSION "TinyWeb/0.0.1"

std::string special_response_body(enum http_status s)
{
    static std::string param1=
        "<html>\r\n"
        "<head><title>";
    
    static std::string param2=
        "</title></head>\r\n"
        "<center><h1>";

    static std::string param3=
        "</h1></center>\r\n"
        "<hr><center>" TINYWEB_VERSION "</center>\r\n"
        "</body>\r\n"
        "</html>\r\n";
    
    std::string code(httpStatusCode(s));
    std::string phrase(httpStatusStr(s));
    std::string tmp = code + " " + phrase;
    if(code == "<invalid>")
        return code;

    std::string result = param1 + tmp + param2 + tmp + param3;
    return result;
}
