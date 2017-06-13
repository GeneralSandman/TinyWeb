#ifndef API_H
#define API_H
#include <unistd.h>
#include <netinet/in.h>
#include <iostream>
#include <string>
#include <map>

std::string cstr2string(const char *str);
std::map<char, std::string> getOption(int argc, char *argv[]);

int Socket(int domain, int type, int protocol);
int Close(int fd);

int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int Listen(int sockfd, int backlog);

ssize_t Write(int sockfd, const std::string &str);

in_addr_t Inet_addr(const std::string &host);
std::string Inet_ntop(int af, const void *src,
                      char *dst, socklen_t size);

#endif