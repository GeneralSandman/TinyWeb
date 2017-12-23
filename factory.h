/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*---class Factory---
*
****************************************
*
*/

#ifndef FACTORY_H
#define FACTORY_H

#include "protocol.h"
#include "connection.h"
#include "callback.h"

#include <map>
#include <set>
#include <iostream>
#include <boost/bind.hpp>

class EventLoop;

/*--------Factory-----------------------*/

class Factory
{
private:
  EventLoop *m_pLoop;
  int m_nNumProts;
  const Protocol *m_pProtocol;
  const std::string m_nProtocolName;
  std::map<Connection *, Protocol *> m_nProtocols;

public:
  Factory(EventLoop *m_pLoop, Protocol *prot);

  ConnectionCallback connectCallback()
  {
    //It is used by Server to set event callback;
    return ConnectionCallback(boost::bind(&Factory::createConnection,
                                          this, _1));
  }

  MessageCallback getMessageCallback()
  {
    //It is used by Server to set event callback;
    return MessageCallback(boost::bind(&Factory::getMessage,
                                       this, _1, _2, _3));
  }

  CloseCallback closeConnectionCallback()
  {
    //It is used by Server to set event callback;
    return CloseCallback(boost::bind(&Factory::lostConnection,
                                     this, _1));
  }

  void createConnection(Connection *newCon);
  void getMessage(Connection *con,
                  Buffer *buf,
                  Time time);
  void lostConnection(Connection *con);

  void closeProtocol(Protocol *);                   //used by protocol
  void closeProtocolAfter(Protocol *, int seconds); //used by protocol

  virtual void buildProtocol(Protocol *newProt);
  // {
  //can be override
  // }
  ~Factory();
};

/*--------ServerFactory-----------------------*/

class ServerFactory : public Factory
{
private:
public:
  ServerFactory(EventLoop*,Protocol *);
  ~ServerFactory();
};

/*--------ClientFactory-----------------------*/

class ClientFactory : public Factory
{
private:
public:
  ClientFactory(EventLoop*,Protocol *);
  ~ClientFactory();
};

// class EchoServerFactory : public ServerFactory
// {
// private:
// public:
//   EchoServerFactory(Protocol *);
//   ~EchoServerFactory();
// };

// class WebServerFactory : public ServerFactory
// {
// private:
// public:
//   WebServerFactory(Protocol *);
//   ~WebServerFactory();
// };

//buildProtocol---return a Protocol
//statredConnecting
//clientConnectionLost
//clientConnectionFailed

#endif // FACTORY_H
