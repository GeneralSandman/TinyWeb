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

class EventLoop;

class Master
{
protected:
  EventLoop *m_pEventLoop;
  int m_nNumber;
  std::string m_nName;

  int m_nListenSocket;

public:
  Master(EventLoop *, int, const std::string &);
  void init()
  {
    m_nListenSocket = createNoBlockSocket();
  }
  void getListenSocket()
  {
    return m_nListenSocket;
  }
  void work();
  ~Master();
};


#endif // !MASTER_H
