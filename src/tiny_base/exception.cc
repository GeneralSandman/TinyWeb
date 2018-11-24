/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

#include <execinfo.h>

#include <tiny_base/exception.h>

Exception::Exception(const char *msg)
    : m_nName(msg)
{
    m_fFillStackTrace();
}

Exception::Exception(const std::string &msg)
    : m_nName(msg)
{
    m_fFillStackTrace();
}

Exception::Exception(const Exception &e)
{
    m_nName = e.m_nName;
    m_nStack = e.m_nStack;
}

void Exception::m_fFillStackTrace()
{
    int size = 200;
    void *buffer[size];
    int nptrs = backtrace(buffer, size);
    char **str = backtrace_symbols(buffer, nptrs);
    if (str)
    {
        for (int i = 0; i < nptrs; i++)
        {
            m_nStack += str[i];
            m_nStack += '\n';
        }
    }
    free(str);
}