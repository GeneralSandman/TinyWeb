#ifndef POLL_H
#define POLL_H
#include "../../api.h"
#include "processpoll.h"
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>

namespace poll
{
class Process
{
public:
  Process() : m_nPid(-1), m_nActive(false) {}
  pid_t m_nPid;
  int m_nPipefd[2];
  bool m_nActive;
};

static int sigpipefd[2];
class Poll
{
private:
  static Poll *m_pInstance;
  Process *m_pProcess;
  int m_nIndex;
  bool m_nStop;
  int m_nListenfd;
  int m_nProcessTotalNumber;
  int m_nProcessActiveNumber;
  int m_nProcessCounter;
  int m_nEpollfd;

  Poll(int listenfd, int process_number = 8);
  void m_fRunChild();
  void m_fRunParent();

public:
  ~Poll() { delete[] m_pProcess; }

  static Poll *create(int listenfd, int process_number = 8)
  {
    if (!m_pInstance)
    {
      m_pInstance = new Poll(listenfd, process_number);
    }
    return m_pInstance;
  }
  void setup_sig_pipe(void);
  void run(void);

  static void signal_handler(int sig)
  {
    int old_error = errno;
    int msg = sig;
    send(sigpipefd[1], (char *)&sig, 1, 0);
    errno = old_error;
    std::cout << "receive signal " << sig << std::endl;
  }
};


class Service
{
private:
  static const int BUF_SIZE = 1024;
  char m_nBuf[BUF_SIZE];

  int m_nConnectFd;
  struct sockaddr_in m_nClientAddr;

public:
  Service() {}
  ~Service();
  void init(int sockfd, const sockaddr_in &client_addr);
  void proc();
};


}

#endif