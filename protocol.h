/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*---class Protocol---
*This class is will used by class Server
*The only thing we should do is to defind a new drive-Protocol
****************************************
*
*
*/

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "callback.h"
#include "time.h"

#include <iostream>
#include <boost/bind.hpp>

class Connection;
class Buffer;

class Protocol
{
private:
public:
  Protocol();
  //used by server
  ConnectionCallback connectCallback()
  {
    return ConnectionCallback(boost::bind(&Protocol::connectionMade,
                                          this, _1));
  }

  MessageCallback getMessageCallback()
  {
    return MessageCallback(boost::bind(&Protocol::dataReceived,
                                       this, _1, _2, _3));
  }

  CloseCallback closeConnectionCallback()
  {
    return CloseCallback(boost::bind(&Protocol::connectionLost,
                                     this, _1));
  }

  //used by user
  virtual void connectionMade(Connection *con)
  {
    std::cout << "(base Protocol) "
              << "get a new connection\n";
  }
  virtual void dataReceived(Connection *con, Buffer *input, Time time)
  {
    std::cout << "(base Protocol) "
              << "get a new message\n";
  }
  virtual void connectionLost(Connection *con)
  {
    std::cout << "(base Protocol) "
              << "lost a connection\n";
  }
  virtual ~Protocol();
};

class EchoProtocol : public Protocol
{
public:
  EchoProtocol();
  void connectionMade(Connection *con)
  {
    std::cout << "(EchoProtocol) "
              << "get a new connection\n";
  }
  void dataReceived(Connection *con, Buffer *input, Time time)
  {
    std::cout << "(EchoProtocol) "
              << "get a new message\n";
  }
  void connectionLost(Connection *con)
  {
    std::cout << "(EchoProtocol) "
              << "lost a connection\n";
  }
  virtual ~EchoProtocol();
};

class WebProtocol : public Protocol
{
public:
  WebProtocol();
  void connectionMade(Connection *con)
  {
    std::cout << "(WebProtocol) "
              << "get a new connection\n";
  }
  void dataReceived(Connection *con, Buffer *input, Time time)
  {
    std::cout << "(WebProtocol) "
              << "get a new message\n";
  }
  void connectionLost(Connection *con)
  {
    std::cout << "(WebProtocol) "
              << "lost a connection\n";
  }
  virtual ~WebProtocol();
};

#endif // !PROTOCOL_H