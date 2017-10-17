/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*---class Protocol---
*This class is will used by class Server
*The only thing we should do is to defind a new drive-Protocol
*This class will is be used to 
****************************************
*
*
*/

/*---class EchoProtocol---
*
****************************************
*
*/

/*---class WebProtocol---
*
****************************************
*
*/

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "callback.h"
#include "connection.h"
#include "parser.h"
#include "buffer.h"
#include "time.h"
#include "api.h"

#include <iostream>
#include <boost/bind.hpp>

class Connection;
class Buffer;

/*----------Protocol------------*/
class Protocol
{
private:
  // Parser m_nParser;

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

/*-------EchoProtocol------------*/
class EchoProtocol : public Protocol
{
public:
  EchoProtocol();
  void connectionMade(Connection *con);
  void dataReceived(Connection *con, Buffer *input, Time time);
  void connectionLost(Connection *con);
  virtual ~EchoProtocol();
};

/*--------WebProtocol-----------*/
class WebProtocol : public Protocol
{
private:
  HttpParser m_nParser;

  void m_fResponse(Connection *con, struct HttpRequest &);

public:
  WebProtocol();
  void connectionMade(Connection *con);
  void dataReceived(Connection *con, Buffer *input, Time time);
  void connectionLost(Connection *con);
  virtual ~WebProtocol();
};

#endif // !PROTOCOL_H