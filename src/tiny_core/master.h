/*
 *Author:GeneralSandman
 *Code:https://github.com/GeneralSandman/TinyWeb
 *E-mail:generalsandman@163.com
 *Web:www.dissigil.cn
 */

/*---XXX---
 *
 ****************************************
 *
 */

#ifndef MASTER_H
#define MASTER_H

#include <memory>
#include <string>

class ProcessPool;
class EventLoop;
class Socket;

class Master {
protected:
    ProcessPool* m_pProcessPool;
    EventLoop* m_pEventLoop;
    int m_nNumber;
    std::string m_nName;
    std::shared_ptr<Socket> m_pListenSocket;

    int status;

public:
    Master(ProcessPool*, EventLoop*, int, const std::string&);
    void init();
    int getListenSocket();
    void work();
    ~Master();
};

#endif // !MASTER_H
