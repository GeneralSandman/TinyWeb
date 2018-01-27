/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*---String class---
*
****************************************
*
*/

#ifndef STRING_H
#define STRING_H

#include "log.h"

class String
{
  public:
    char *data;
    int len;

    String()
    {
        LOG(Debug) << "class String constructor\n";
    }

    ~String()
    {
        LOG(Debug) << "class String destructor\n";
    }
};

class KeyValue
{
  private:
    String m_nKey;
    String m_nValue;

  public:
    KeyValue();
    ~KeyValue();
};

#endif // !STRING_H