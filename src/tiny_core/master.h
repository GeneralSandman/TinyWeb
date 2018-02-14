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

#ifndef MASTER_H
#define MASTER_H

#include <string>

class ProcessPool;
class EventLoop;

class Master
{
protected:
  ProcessPool *m_pProcessPool;
  EventLoop *m_pEventLoop;
  int m_nNumber;
  std::string m_nName;
  int m_nListenSocket;

public:
  Master(ProcessPool *, EventLoop *, int, const std::string &);
  void init();
  int getListenSocket();
  void work();
  ~Master();
};

#endif // !MASTER_H
