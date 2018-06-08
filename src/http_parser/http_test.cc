/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*---test case of http.h---
*How to use http.h.
****************************************
*
*/

#include "http.h"

#include <iostream>

using namespace std;

void testStatus()
{
    cout << "------test Http Status------\n";
    //It is just a test,we don't use ELEM_AT() directly.
    for (int i = 0; i <= ARRAY_SIZE(statusStrings); i++)
    {
        cout << i << ":" << ELEM_AT(statusStrings, i, "<unknown>") << endl;
    }

    //Invoke httpStatusStr by enum http_status.
    //Using httpStatusStr() is recommended.
    cout << HTTP_STATUS_CONTINUE << httpStatusStr(HTTP_STATUS_CONTINUE) << endl;
    cout << HTTP_STATUS_ACCEPTED << httpStatusStr(HTTP_STATUS_ACCEPTED) << endl;
}

void testMethod()
{
    cout << "------test Http Method------\n";
    //It is just a test,we don't use ELEM_AT() directly.
    for (int i = 0; i <= ARRAY_SIZE(methodStrings); i++)
    {
        cout << i << ":" << ELEM_AT(methodStrings, i, "<unknown>") << endl;
    }

    //Invoke httpMethodStr by parameter enum http_method.
    //Using httpMethodStr() is recommended.
    cout << HTTP_DELETE << ":" << httpMethodStr(HTTP_DELETE) << endl;
    cout << HTTP_GET << ":" << httpMethodStr(HTTP_GET) << endl;
    cout << HTTP_POST << ":" << httpMethodStr(HTTP_POST) << endl;
}

void testErrno()
{
    cout << "------test Http Errno------\n";
    //It is just a test,we don't use ELEM_AT() directly.
    for (int i = 0; i <= ARRAY_SIZE(errnoStrings); i++)
    {
        cout << i << ":" << ELEM_AT(errnoStrings, i, "<unknown>") << endl;
    }

    //Invoke httpErrnoStr by parameter enum http_errno.
    //Using httpErrnoStr() is recommended.
    cout << HPE_CB_message_begin << ":" << httpErrnoStr(HPE_CB_message_begin) << endl;
    cout << HPE_OK << ":" << httpErrnoStr(HPE_OK) << endl;
    cout << HPE_CB_url << ":" << httpErrnoStr(HPE_CB_url) << endl;
}

int main()
{
    testStatus();
    testMethod();
    testErrno();

    return 0;
}