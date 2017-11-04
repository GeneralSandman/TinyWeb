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

#ifndef WORKER_H
#define WORKER_H

#include <string>

class Worker
{
  protected:
    int m_nNumber;
    std::string m_nName;

  public:
    Worker(int, std::string);
    Worker(const Worker &);
    Worker &operator=(const Worker &);
    virtual void work();
    virtual ~Worker();
};

class HttpListenWorker : public Worker
{
  public:
    HttpListenWorker(int, std::string);
    virtual void work();
    ~HttpListenWorker();
};

#endif // !WORKER_H
