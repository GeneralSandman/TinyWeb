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
 
#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <mqueue.h>

class MessageQueue
{
  private:
    mqd_t m_nMQFd;

  public:
    MessageQueue();
    void notify();
    void notify_all();
    void wait();
    
    void recev();
    void send();
    ~MessageQueue();
};

#endif