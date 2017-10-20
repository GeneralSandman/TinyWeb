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

#ifndef RESPONSER_H
#define RESPONSER_H

#include "http.h"

class WebProtocol;

class HttpResponser
{
private:
  WebProtocol *m_pProtocol;

  size_t m_fGetFileSize(const std::string &f);
  size_t m_fWriteFile(const std::string &f);

public:
  HttpResponser(WebProtocol *prot);
  void response(const struct HttpRequest &);
  ~HttpResponser();
};

#endif // !RESPONSE_H
