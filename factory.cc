/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*---XXX---
*
****************************************
*
*/

#include "factory.h"
#include "protocol.h"
#include "log.h"

//---ServerFactory api--------------//
ServerFactory::ServerFactory()
{
    LOG(Debug) << "class ServerFactory constructor\n";
}

ServerFactory::~ServerFactory()
{
    LOG(Debug) << "class ServerFactory destructor\n";
}

//---EchoServerFactory api--------------//
EchoServerFactory::EchoServerFactory()
{
    LOG(Debug) << "class EchoServerFactory constructor\n";
}

EchoServerFactory::~EchoServerFactory()
{
    LOG(Debug) << "class EchoServerFactory destructor\n";
}

//---WebServerFactory api--------------//
WebServerFactory::WebServerFactory()
{
    LOG(Debug) << "class WebServerFactory constructor\n";
}

WebServerFactory::~WebServerFactory()
{
    LOG(Debug) << "class WebServerFactory destructor\n";
}