/*
 *Author:GeneralSandman
 *Code:https://github.com/GeneralSandman/TinyWeb
 *E-mail:generalsandman@163.com
 *Web:www.dissigil.cn
 */

/*---XXX---
 *
 ****************************************
 *
 */

#include <tiny_base/log.h>
#include <tiny_core/factory.h>
#include <tiny_http/http_factory.h>

WebServerFactory::WebServerFactory(EventLoop* loop, Protocol* protocol)
    : Factory(loop, protocol)
{
    LOG(Debug) << "class WebServerFactory constructor\n";
}

void WebServerFactory::buildProtocol(Protocol* newProt)
{
    //set member of Protocol.
}

WebServerFactory::~WebServerFactory()
{
    LOG(Debug) << "class WebServerFactory destructor\n";
}
