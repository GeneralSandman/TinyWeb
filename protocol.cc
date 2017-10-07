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

#include "protocol.h"
#include "log.h"

Protocol::Protocol()
{
    LOG(Debug) << "class Protocol constructor\n";
}

Protocol::~Protocol()
{
    LOG(Debug) << "class Protocol destructor\n";
}

WebProtocol::WebProtocol()
{
    LOG(Debug) << "class WebProtocol constructor\n";
}

WebProtocol::~WebProtocol()
{
    LOG(Debug) << "class WebProtocol destructor\n";
}