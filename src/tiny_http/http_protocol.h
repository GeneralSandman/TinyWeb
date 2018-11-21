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
#define HTTP_PROTOCOL_Hs


#include <tiny_base/log.h>
#include <tiny_core/http_protocol.h>

/*--------WebProtocol-----------*/

class WebProtocol : public Protocol
{
private:
  bool m_nKeepAlive;
  HttpParser m_nParser;
  HttpResponser m_nResponser;

public:
  WebProtocol();
  virtual void connectionMade();
  virtual void dataReceived(const std::string &);
  virtual void connectionLost();
  virtual ~WebProtocol();
};

#endif // !HTTP_PROTOCOL_H

