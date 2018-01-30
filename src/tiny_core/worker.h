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

class EventLoop;

class Worker
{
protected:
  EventLoop *m_pEventLoop;
  int m_nNumber;
  std::string m_nName;

public:
  Worker(EventLoop *, int, const std::string &);
  virtual void work();
  virtual ~Worker();
};

class Master : public Worker
{
public:
  Master(EventLoop *, int, const std::string &);
  virtual void work();
  virtual ~Master();
};

class Slave : public Worker
{
public:
  Slave(EventLoop *, int, const std::string &);
  virtual void work();
  virtual ~Slave();
};

/////////
class HttpListenWorker : public Worker
{
public:
  HttpListenWorker(EventLoop*,int, std::string);
  virtual void work();
  ~HttpListenWorker();
};

#endif // !WORKER_H
