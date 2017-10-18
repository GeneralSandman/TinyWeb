/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*---Factory class---
*This class is used to maintain context
****************************************
*
*/

#ifndef FACTORY_H
#define FACTORY_H

#include "protocol.h"

class ServerFactory
{
private:
public:
  ServerFactory();
  ~ServerFactory();
};

class EchoServerFactory : public ServerFactory
{
private:
public:
  EchoServerFactory();
  ~EchoServerFactory();
};

class WebServerFactory : public ServerFactory
{
private:
public:
  WebServerFactory();
  ~WebServerFactory();
};

//buildProtocol---return a Protocol
//statredConnecting
//clientConnectionLost
//clientConnectionFailed

#endif // FACTORY_H
