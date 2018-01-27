/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*---String class---
*String don't manage memory.
****************************************
*
*/

#ifndef STRING_H
#define STRING_H

#include "log.h"

#include <cstring>

class String
{
public:
  char *data;
  int len;

  String()
  {
    data = nullptr;
    len = 0;
  }

  String(const String &s)
  {
    data = s.data;
    len = s.len;
  }

  String(char *str)
  {
    data = str;
    len = strlen(str);
  }

  String(char *str, int l)
  {
    data = str;
    len = l;
  }

  void setNull()
  {
    data = nullptr;
    len = 0;
  }

  void swap(String str)
  {
    std::swap(data, str.data);
    std::swap(len, str.len);
  }

  String &operator=(const String &s)
  {
    data = s.data;
    len = s.len;

    return *this;
  }

  friend bool operator>(const String &a, const String &b)
  {
    return a.len > b.len;
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