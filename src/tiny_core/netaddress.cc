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

#include <tiny_core/netaddress.h>
#include <tiny_base/api.h>

#include <string>
#include <strings.h>

NetAddress::NetAddress()
{
    bzero(&m_nAddress, sizeof(m_nAddress));
}

NetAddress::NetAddress(const NetAddress &address)
{
    m_nAddress = address.m_nAddress;
}

NetAddress::NetAddress(int port)
{
    bzero(&m_nAddress, sizeof m_nAddress);
    m_nAddress.sin_family = AF_INET;
    m_nAddress.sin_addr.s_addr = hostToNet32(0);
    m_nAddress.sin_port = hostToNet16(port);
}

NetAddress::NetAddress(const std::string &ip, int port)
{
    bzero(&m_nAddress, sizeof(m_nAddress));
    IpPortToSockAddr(ip.c_str(), port, &m_nAddress);
}

NetAddress::NetAddress(const std::string &ipport)
{
    bzero(&m_nAddress, sizeof(m_nAddress));
    //127.0.0.1:9999
    std::string ip;
    std::string port;

    bool flag = false;
    for (unsigned int i = 0; i < ipport.size(); i++)
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

NetAddress &NetAddress::operator=(const NetAddress &address)
{
    if (this != &address)
        m_nAddress = address.m_nAddress;
    return (*this);
}

std::string NetAddress::getIp() const
{
    char ip[INET_ADDRSTRLEN];
    int port;
    SockAddrToIpPort(ip, INET_ADDRSTRLEN, port, &m_nAddress);
    std::string res;
    for (int i = 0; ip[i] != '\0'; i++)
        res += ip[i];
    return res;
}

int NetAddress::getPort() const
{
    char ip[INET_ADDRSTRLEN];
    int port;
    SockAddrToIpPort(ip, INET_ADDRSTRLEN, port, &m_nAddress);
    return port;
}

std::string NetAddress::getIpPort() const
{
    char ip[INET_ADDRSTRLEN];
    int port;
    SockAddrToIpPort(ip, INET_ADDRSTRLEN, port, &m_nAddress);
    std::string res;
    for (int i = 0; ip[i] != '\0'; i++)
        res += ip[i];
    res += ":";
    res += std::to_string(port);
    return res;
}

NetAddress::~NetAddress()
{
}
