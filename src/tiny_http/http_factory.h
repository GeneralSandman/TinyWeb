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

#ifndef HTTP_FACTORY_H
#define HTTP_FACTORY_H

#include <tiny_base/log.h>
#include <tiny_core/factory.h>

class WebServerFactory : public Factory {
private:
public:
    WebServerFactory(EventLoop*, Protocol*);
    virtual void buildProtocol(Protocol* newProt);
    virtual ~WebServerFactory();
};

#endif // !HTTP_FACTORY_H
