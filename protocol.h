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
#include "log.h"

#include <iostream>
#include <sstream>
#include <boost/bind.hpp>
#include <typeinfo>

class Connection;
class Buffer;

/*----------Protocol------------*/
// class Protocol
// {
// private:
//   // Parser m_nParser;

// public:
//   Protocol();
//   //used by server
//   ConnectionCallback connectCallback()
//   {
//     return ConnectionCallback(boost::bind(&Protocol::connectionMade,
//                                           this, _1));
//   }

//   MessageCallback getMessageCallback()
//   {
//     return MessageCallback(boost::bind(&Protocol::dataReceived,
//                                        this, _1, _2, _3));
//   }

//   CloseCallback closeConnectionCallback()
//   {
//     return CloseCallback(boost::bind(&Protocol::connectionLost,
//                                      this, _1));
//   }

//   //used by user
//   virtual void connectionMade(Connection *con)
//   {
//     std::cout << "(base Protocol) "
//               << "get a new connection\n";
//   }
//   virtual void dataReceived(Connection *con, Buffer *input, Time time)
//   {
//     std::cout << "(base Protocol) "
//               << "get a new message\n";
//   }
//   virtual void connectionLost(Connection *con)
//   {
//     std::cout << "(base Protocol) "
//               << "lost a connection\n";
//   }
//   virtual ~Protocol();
// };

class Factory;
class Protocol;

// class Reflect
// {

// };

std::string getName(Protocol *p)
{
  std::stringstream s;
  s << std::typeid(*p).name();
  std::string tmp(s.str().c_str());

  std::string res;
  for (int i = 0; i < tmp.size(); i++)
    if (!(tmp[i] >= 48 && tmp[i] <= 57))
      res += tmp[i];
  return res;
}

class Protocol
{
private:
  Factory *m_pFactory;
  int m_nNumber;
  Connection *m_pConnection;

  // virtual Protocol *get

public:
  Protocol();
  void makeConnection();
  void getMessage(const std::string &);
  void loseConnection();

  void sendMessage(const std::string &);

  virtual void connectionMade();
  virtual void dataReceived(const std::string &);
  virtual void connectionLost();
  virtual ~Protocol();

  friend class Factory;
};

/*-------DiscardProtocol------------*/
class DiscardProtocol : public Protocol
{
public:
  DiscardProtocol();
  virtual void connectionMade();
  virtual void dataReceived(const std::string &);
  virtual void connectionLost();
  virtual ~DiscardProtocol();
};

/*-------EchoProtocol------------*/
class EchoProtocol : public Protocol
{
public:
  EchoProtocol();
  virtual void connectionMade();
  virtual void dataReceived(const std::string &);
  virtual void connectionLost();
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
  virtual void connectionMade(Connection *con);
  virtual void dataReceived(Connection *con, Buffer *input, Time time);
  virtual void connectionLost(Connection *con);
  virtual ~WebProtocol();
};

#endif // !PROTOCOL_H