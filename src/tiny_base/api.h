/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*---header api.h/cc---
*This file is a set of apis which invoke system call and add
*error detection.
*TinyWeb don't invoke system call directly but use functions
provied by this file.
****************************************
*
*/
#ifndef API_H
#define API_H

#include <unistd.h>
#include <netinet/in.h>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <regex>


pid_t gettid();

void handle_error(const char *msg);
void handle_error_s(const std::string &msg);
void setnonblocking(int sock);

void getLines(const std::string &s, std::vector<std::string> &res, std::string &resOpenLine);
std::string cstr2string(const char *str);
void splitString(const std::string &, const std::string &, std::vector<std::string> &);
void eraseSpace(std::string &);
void eraseAllSpace(std::string &);
std::map<char, std::string> getOption(int argc, char *argv[], const struct option *longopts, const char * short_options);

int createSocket(int domain, int type, int protocol);
int createSocket();

int getSocketError(int sockfd);

int createNoBlockSocket(int, int, int);
int createNoBlockSocket();
void Close(int fd);

int Bind(int sockfd, const struct sockaddr_in *addr, socklen_t addrlen = sizeof(struct sockaddr_in));
int Listen(int sockfd, int backlog);
int Accept(int sockfd, struct sockaddr_in *peraddr);
int Connect(int sockfd, struct sockaddr_in *peraddr);

void ShutdownWrite(int sockfd);

int Open(const char *pathname, int flags, mode_t mode);
int Stat(const char *pathname, struct stat *buf);
void *Mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset);
int Munmap(void *start, size_t length);

ssize_t writeString(int sockfd, const std::string &str);
ssize_t Rio_writen(int fd, void *usrbuf, size_t n);
ssize_t writeHtml(int sockfd, const std::string &filename);

int get_line(int sock, char *buf, int size);

in_addr_t Inet_addr(const std::string &host);
std::string Inet_ntop(int af, const void *src,
                      char *dst, socklen_t size);

void IpPortToSockAddr(const char *, int, struct sockaddr_in *res);
void SockAddrToIpPort(char *ip, int size, int &port, const struct sockaddr_in *src);

int setNoBlock(int fd);
int setCLOEXEC(int fd);
int setSocketReuseAddress(int fd);

inline void setTcpDelay(int fd, bool on);
void enableTcpDelay(int fd);
void disableTcpDelay(int fd);

inline void setTcpKeepAlive(int fd, bool on);
void enableTcpKeepAlive(int fd);
void disableTcpKeepAlive(int fd);

struct sockaddr_in getLocalAddr(int sockfd);

void epoll_addfd(int epfd, int fd, int events);
void epoll_modfd(int epfd, int fd, int events);
void epoll_removefd(int epfd, int fd);

typedef void (*sighandler_t)(int);
void add_signal(int sign, sighandler_t handler);
void remove_signal(int sign);

////////////////////// socket api////////////////////

inline uint16_t hostToNet16(uint16_t a)
{
    return htons(a);
}
inline uint32_t hostToNet32(uint32_t a)
{
    return htonl(a);
}

inline uint16_t netToHost16(uint16_t a)
{
    return ntohs(a);
}
inline uint32_t netToHost32(uint32_t a)
{
    return ntohl(a);
}

inline bool regex_match(const std::string& str, const std::string& pattern)
{
    bool res;
    try {
        std::regex reg(pattern);
        res = std::regex_match(str, reg);
    } catch (const std::exception& ex) {
        return false;
    }
    return res;
}

#endif