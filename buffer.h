/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*---class Buffer---
*
****************************************
*
*put(fd):read date from fd and store into vector<char>
*get():fetch readable date
*/

#ifndef BUFFER_H
#define BUFFER_H

#include <vector>
#include <string>
#include <unistd.h>

class Buffer
{
private:
  std::vector<char> m_nDatas;
  int m_nReadIndex;
  int m_nWriteIndex;

  size_t m_fPrependableBytes() const;
  size_t m_fReadableBytes() const;
  size_t m_fWriteableBytes() const;

  char *m_fBegin();
  const char *m_fBegin() const;

  char *m_fReadableBegin();
  const char *m_fReadableBegin() const;
  char *m_fWriteableBegin();
  const char *m_fWriteableBegin() const;

  void m_fEnsureWritableBytes(size_t len)
  {
    if (m_fWriteableBytes() < len)
      m_fMakeSpace(len);
  }
  void m_fMakeSpace(size_t len);

  void m_fClearAll()
  {
    m_nReadIndex = 8;
    m_nWriteIndex = 8;
  }

public:
  Buffer();
  size_t put(int fd);
  std::string get(size_t len)
  {
    if (len > m_fReadableBytes())
      len = m_fReadableBytes();
    std::string result(m_fBegin(), len);
    //FIXME:
    return result;
  }
  std::string getAll()
  {
    return get(m_fReadableBytes());
  }
  void append(const std::string &str)
  {
    append(str.c_str(), str.size());
  }

  void append(const char *str, int len)
  {
    std::copy(str, str + len, m_fWriteableBegin());
    m_nWriteIndex += len;
  }

  ~Buffer();
};

#endif // !BUFFER_H
