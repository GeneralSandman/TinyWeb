#ifndef NET_ADDRESS_H
#define NET_ADDRESS_H

#include <netinet/in.h>
#include <string>

class NetAddress
{
private:
  struct sockaddr_in m_nAddress;

public:
  NetAddress(int port);
  NetAddress(const std::string &ip, int port);
  NetAddress(const std::string &ipport);
  NetAddress(struct sockaddr_in &addr);

  std::string getIp();
  std::string getPort();
  std::string getIpPort();

  const struct sockaddr_in getAddr() { return m_nAddress; }
  void setAddr(struct sockaddr_in a) { m_nAddress = a; }

  ~NetAddress();
};

#endif // !NET_ADDRESS_H
