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

#ifndef PROCESS_POOL_H
#define PROCESS_POOL_H

#include <tiny_base/api.h>
#include <tiny_base/buffer.h>
#include <tiny_base/log.h>
#include <tiny_base/signalmanager.h>
#include <tiny_base/sync.h>
#include <tiny_core/process.h>

#include <map>
#include <memory>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>

#include <tiny_core/status.h>
extern int status_quit_softly; //QUIT
extern int status_terminate;   //TERM,INT
extern int status_exiting;
extern int status_reconfigure; //HUP,reboot
extern int status_child_quit;  //CHLD

class Master;
class SocketPair;
class EventLoop;

struct pair {
    int d1;
    int d2;
};

void test_parent_MessageCallback(Connection* con, Buffer* buf, Time time);
void test_parent_CloseCallback(Connection* con);

void test_parent_period_print(void);

class ProcessPool {

private:
    pid_t m_nPid;
    std::shared_ptr<EventLoop> m_pEventLoop;
    std::shared_ptr<Master> m_pMaster;
    std::shared_ptr<Process> m_pProcess;

    std::vector<std::shared_ptr<SocketPair>> m_nPipes;
    std::vector<pid_t> m_nPids;
    int m_nProcessNum;

    typedef std::pair<NetAddress, Socket*> NetSocketPair;
    std::vector<NetSocketPair> m_nListenSocketFds;

    SignalManager m_nSignalManager;

    int status;

    std::shared_ptr<Sync> m_pSync;

    static ProcessPool* m_pPoolInstance;

    static void parentSignalHandler(int sign)
    {
        m_fSignalHandler(m_pPoolInstance, sign);
    }

    static void m_fSignalHandler(ProcessPool* pool, int sign)
    {
        if (nullptr == pool)
            return;

        LOG(Debug) << "[parent] signal manager get signal(" << sign << ")\n";
        int status;
        pid_t pid;
        switch (sign) {

        case SIGINT:
        case SIGTERM:
            status_terminate = 1;
            LOG(Debug) << "[parent] will terminate all slave process\n";
            break;

        case SIGQUIT:
            status_quit_softly = 1;
            LOG(Debug) << "[parent] quit softly\n";
        case SIGCHLD:
            status_child_quit = 1;
            pid = waitpid(-1, &status, WNOHANG);
            LOG(Debug) << "[parent] collect information from child(" << pid << ")\n";
            pool->m_fDestoryProcess(pid);
            break;
            // case SIGHUP:
            //   status_reconfigure = 1;
            //   std::cout << "[parent]:reconfigure\n";
            //   //kill childern softly and create new process
            //   break;

        case SIGPIPE:
            break;

        case SIGUSR1:
            status_restart = 1;
            LOG(Debug) << "[parent] restart \n";
            break;

        case SIGUSR2:
            status_reconfigure = 1;
            LOG(Debug) << "[parent] reload config\n";
            break;

        default:
            break;
        }
    }

    void m_fDestoryProcess(pid_t pid)
    {
        unsigned int index = 0;
        for (; index < m_nPids.size(); index++) {
            if (m_nPids[index] == pid)
                break;
        }
        // assert(index != m_nPids.size());
        if (index == m_nPids.size()) {
            LOG(Debug) << "process(" << pid << ") doesn't exist\n";
            return;
        }

        m_nPipes[index]->clearSocket();
        m_nPipes.erase(m_nPipes.begin() + index);
        m_nPids.erase(m_nPids.begin() + index);
        m_nProcessNum--;
    }

public:
    ProcessPool();
    void init();
    void createProcess(int nums);
    inline int processNum() { return m_nProcessNum; }
    void setSignalHandlers();
    void start();
    void killAll();
    void killSoftly();

    void writeToShareMemory(const std::string& data);
    std::string readFromSharedMemory(void);

    ~ProcessPool();
    friend class Process;
};

#endif
