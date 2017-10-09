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

public:
  Buffer();
  size_t put(int fd);
  std::string get();

  ~Buffer();
};

#endif // !BUFFER_H
