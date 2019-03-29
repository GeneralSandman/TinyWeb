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
#include <vector>

class ProcessPool;
class EventLoop;
class NetAddress;
class Socket;

class Master {
protected:
    ProcessPool* m_pProcessPool;
    EventLoop* m_pEventLoop;
    int m_nNumber;
    std::string m_nName;

    typedef std::pair<NetAddress, Socket*> NetSocketPair;
    std::vector<NetSocketPair> m_nListenSockets;

    bool m_nStart;

public:
    Master(ProcessPool*, EventLoop*, int, const std::string&);
    void init();
    void getListenSockets(std::vector<NetSocketPair>& result);
    void work();
    ~Master();
};

#endif // !MASTER_H
