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

#ifndef NET_ADDRESS_H
#define NET_ADDRESS_H

#include <netinet/in.h>
#include <string>

class NetAddress
{
private:
  struct sockaddr_in m_nAddress;

public:
  NetAddress();
  NetAddress(const NetAddress &);
  NetAddress(int port);
  NetAddress(const std::string &ip, int port);
  NetAddress(const std::string &ipport);
  NetAddress(struct sockaddr_in &addr);
  NetAddress &operator=(const NetAddress &);

  std::string getIp() const;
  int getPort() const;
  std::string getIpPort() const;

  const struct sockaddr_in getAddr() const { return m_nAddress; }
  void setAddr(struct sockaddr_in a) { m_nAddress = a; }

  ~NetAddress();
};

#endif // !NET_ADDRESS_H
