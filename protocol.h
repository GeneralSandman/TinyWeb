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

#ifndef PROTOCOL_H
#define PROTOCOL_H

class Protocol
{
private:
public:
  Protocol();
  virtual ~Protocol();
};

class WebProtocol : public Protocol
{
private:
public:
  WebProtocol();
  virtual ~WebProtocol();
};

#endif // !PROTOCOL_H