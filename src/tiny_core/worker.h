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
  Worker(int, const std::string&);
  virtual void work();
  virtual ~Worker();
};

class Master : public Worker
{
public:
  Master(int, const std::string &);
  virtual void work();
  virtual ~Master();
};

class Slave : public Worker
{
public:
  Slave(int, const std::string &);
  virtual void work();
  virtual ~Slave();
};

/////////
class HttpListenWorker : public Worker
{
public:
  HttpListenWorker(int, std::string);
  virtual void work();
  ~HttpListenWorker();
};

#endif // !WORKER_H
