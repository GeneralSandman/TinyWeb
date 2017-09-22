#ifndef CHANNEL_H
#define CHANNEL_H

class EventLoop;

class Channel
{
  private:
    EventLoop *m_pEventLoop;
    int m_nFd;
    //readCallback;
    //writeCallback;
    //closeCallback;
    //errorCallback;
    int m_nEvent;
    int m_nREvent;

  public:
    Channel(EventLoop *, int fd);
    void handleEvent();

    void setReadCallback();
    void setWriteCallback();
    void setCloseCallback();
    void setErrorCallback();

    void enableRead();
    void enableWrite();
    void enableAll();

    void disableRead();
    void disableWrite();
    void disableAll();

    ~Channel();
};

#endif