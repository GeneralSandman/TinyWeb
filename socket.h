#ifndef SOCKET_H
#define SOCKET_H

class Socket
{
  private:
    int m_nFd;

  public:
    Socket(int);
    void bindAddress();
    void listen();
    int accept();
    ~Socket();
};

#endif