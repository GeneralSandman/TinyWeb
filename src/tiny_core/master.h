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
#include <memory>

class ProcessPool;
class EventLoop;
class Socket;

class Master
{
protected:
  ProcessPool *m_pProcessPool;
  EventLoop *m_pEventLoop;
  int m_nNumber;
  std::string m_nName;
  std::shared_ptr<Socket> m_pListenSocket;

public:
  Master(ProcessPool *, EventLoop *, int, const std::string &);
  void init();
  int getListenSocket();
  void work();
  ~Master();
};

#endif // !MASTER_H
