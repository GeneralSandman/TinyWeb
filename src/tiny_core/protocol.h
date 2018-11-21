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

#include <tiny_core/callback.h>
#include <tiny_core/connection.h>
#include <tiny_http/parser.h>
#include <tiny_http/responser.h>
#include <tiny_base/buffer.h>
#include <tiny_core/time.h>
#include <tiny_base/api.h>
#include <tiny_base/log.h>

#include <iostream>
#include <sstream>
#include <boost/bind.hpp>
#include <typeinfo>
#include <map>

class Connection;
class Buffer;
class Factory;
class Protocol;

typedef Protocol *(*createProtocol)(void);

class Reflect
{
private:
  static std::map<std::string, createProtocol> m_nProts;
  Reflect()
  {
    std::cout << "class Reflect constructor\n";
  }

public:
  static Reflect &getReflectInstance()
  {
    static Reflect r;
    return r;
  }
  Protocol *getProtocolByName(const std::string &name)
  {
    auto p = m_nProts.find(name);
    if (p == m_nProts.end())
      return nullptr;
    else
      return p->second();
  }
  void regist(const std::string &name, createProtocol f)
  {
    m_nProts[name] = f;
  }
  ~Reflect()
  {
    std::cout << "class Reflect destructor\n";
  }
};

class RegistAction
{
public:
  RegistAction(const std::string &name, createProtocol f)
  {
    Reflect::getReflectInstance().regist(name, f);
  }
};

#define RegistProtocol(ProtocolName)    \
  ProtocolName *creator##ProtocolName() \
  {                                     \
    return new ProtocolName;            \
  }                                     \
  RegistAction regist##ProtocolName(#ProtocolName, (createProtocol)creator##ProtocolName)

std::string getName(Protocol *p);
Protocol *getInstanceByPointer(const Protocol *p);


/*-------Protocol------------*/

class Protocol
{
public:
  Factory *m_pFactory;
  int m_nNumber;
  Connection *m_pConnection;

public:
  Protocol();
  void makeConnection();
  void getMessage(const std::string &);
  void writeComplete();
  void loseConnection();

  void sendMessage(const std::string &);
  void closeProtocol();
  void closeProtocolAfter(int seconds);

  virtual void connectionMade();
  virtual void dataReceived(const std::string &);
  virtual void writeCompletely();
  virtual void connectionLost();
  virtual ~Protocol();

  friend class Factory;
};

#endif // !PROTOCOL_H