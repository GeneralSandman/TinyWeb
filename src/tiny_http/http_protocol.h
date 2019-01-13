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

#ifndef HTTP_PROTOCOL_H
#define HTTP_PROTOCOL_H


#include <tiny_base/log.h>
#include <tiny_core/protocol.h>
#include <tiny_http/http_model_fcgi.h> 

// Using for web server.
class WebProtocol : public Protocol
{
private:
  bool m_nKeepAlive;

public:
  WebProtocol();
  virtual void connectionMade();
  virtual void dataReceived(const std::string &);
  virtual void connectionLost();
  virtual ~WebProtocol();
};

// Using for fcgi client.
class FcgiClientProtocol : public Protocol
{
private:
  bool m_nKeepAlive;

  HttpModelFcgi fcgiModel;

public:
  FcgiClientProtocol();
  virtual void connectionMade();
  virtual void dataReceived(const std::string &);
  virtual void connectionLost();
  virtual ~FcgiClientProtocol();
};

#endif // !HTTP_PROTOCOL_H
