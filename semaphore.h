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

#ifndef SEMAPHORE_H
#define SEMAPHORE_H

class Semphore
{
  private:
  public:
    Semphore();
    void wait();
    void post();
    ~Semphore();
};

#endif // !SEMAPHORE_H
