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

#ifndef BUFFER_H
#define BUFFER_H

#include <vector>

class Buffer
{
  private:
    std::vector<char> m_nDatas;
    int m_nReadIndex;
    int m_nWriteIndex;

  public:
    Buffer();
    ~Buffer();
};

#endif // !BUFFER_H
