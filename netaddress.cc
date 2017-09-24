#include "netaddress.h"
#include "api.h"

#include <string>
#include <strings.h>

NetAddress::NetAddress(int port)
{
    bzero(&m_nAddress, sizeof m_nAddress);
    m_nAddress.sin_family = AF_INET;
    // m_nAddress.sin_addr.s_addr = hostToNet16(INADDR_ANY);
    m_nAddress.sin_port = hostToNet16(port);
}

NetAddress::NetAddress(const std::string &ip, int port)
{
    IpPortToSockAddr(ip.c_str(), port, &m_nAddress);
}

NetAddress::NetAddress(const std::string &ipport)
{
    //127.0.0.1:9999
    std::string ip;
    std::string port;

    bool flag = false;
    for (int i = 0; i < ipport.size(); i++)
    {
        if (ipport[i] == ':')
        {
            flag = true;
            continue;
        }
        if (flag)
            port += ipport[i];
        else
            ip += ipport[i];
    }

    IpPortToSockAddr(ip.c_str(), atoi(port.c_str()), &m_nAddress);
}

NetAddress::NetAddress(struct sockaddr_in &addr)
{
    m_nAddress = addr;
}

std::string NetAddress::getIp()
{
    std::string ip;

    return ip;
}

std::string NetAddress::getPort()
{
}

std::string NetAddress::getIpPort()
{
    // SockAddrToIpPort(,,,&m_nAddress);
}

NetAddress::~NetAddress()
{
}
