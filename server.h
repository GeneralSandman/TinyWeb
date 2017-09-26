#ifndef SERVER_H
#define SERVER_H

#include "channel.h"
#include "accepter.h"
#include "netaddress.h"

#include <boost/function.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include <vector>
#include <map>

typedef boost::function<void()> connectCallback;
typedef boost::function<void()> messageCallback;

class EventLoop;
class Connection;

class Server
{
private:
  bool m_nStarted;
  int m_nConNum;
  NetAddress m_nListenAddress;
  EventLoop *m_pEventLoop;
  Accepter m_nAccepter;
  boost::ptr_vector<Connection> m_nConnections;
  connectCallback m_nConnectCallback;
  messageCallback m_nMessageCallback;

  void m_fHandleRead(int, const NetAddress &);

public:
  Server(EventLoop *, const NetAddress &);
  void setConenctCallback(connectCallback c)
  {
    m_nConnectCallback = c;
  }
  void setMessageCallback(messageCallback c)
  {
    m_nMessageCallback = c;
  }
  void start();
  ~Server();
};

#endif // !SERVER_H
