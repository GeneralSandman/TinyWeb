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

#include "http.h"

/*Array statusStrings is used by httpStatueStr()*/
static const char *statusStrings[] =
    {
#define XX(num, name, string) #string,
        HTTP_STATUS_MAP(XX)
#undef XX
};

const char *httpStatusStr(enum http_status s)
{
    return ELEM_AT(statusStrings, s, "<invalid>");
}

/*Array methodString is used by httpMethodStr()*/
static const char *methodStrings[] =
    {
#define XX(num, name, string) #string,
        HTTP_METHOD_MAP(XX)
#undef XX
};

const char *httpMethodStr(enum http_method m)
{
    return ELEM_AT(methodStrings, m, "<unknown>");
}

/*Array errnoStrings is used by httpErrnoStr()*/
static const char *errnoStrings[] =
    {
#define XX(name, string) #string,
        HTTP_ERRNO_MAP(XX)
#undef XX
};

const char *httpErrnoStr(enum http_errno e)
{
    return ELEM_AT(errnoStrings, e, "<invalid>");
}