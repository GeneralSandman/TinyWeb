#include "api.h"
#include <iostream>
#include <string.h>
#include <map>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <getopt.h>

std::string cstr2string(const char *str)
{
    std::string result;
    while (*str != '\0')
    {
        result += *str;
        str++;
    }
    return result;
}

std::map<char, std::string> getOption(int argc, char *argv[])
{
    std::map<char, std::string> result;
    struct option longopts[] = {
        {"port", 1, NULL, 'p'},
        {0, 0, 0, 0},
    };
    const char *short_options = "p:";
    int c;
    while ((c = getopt_long(argc, argv, short_options, longopts, NULL)) != -1)
    {
        result[c] = cstr2string(optarg);
    }

    return result;
}

int Socket(int domain, int type, int protocol)
{
    int res = socket(domain, type, protocol);
    if (res < 0)
        std::cout << "socket error\n";

    return res;
}

int Close(int fd)
{
    int res = close(fd);
    if (res < 0)
        std::cout << "close error\n";
    return res;
}

int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{

    while (bind(sockfd, addr, addrlen) < 0)
    {
        std::cout << "bind error,try bind again\n";
    }
}

int Listen(int sockfd, int backlog)
{
    if (listen(sockfd, 20) < 0)
        std::cout << "listen error\n";
}

ssize_t Write(int sockfd, const std::string &str)
{
    char *tmp = new char[str.size() + 1];
    char *head = tmp;
    for (auto t : str)
        *(head++) = t;

    ssize_t result = write(sockfd, tmp, strlen(tmp));
    delete[] tmp;
    return result;
}

in_addr_t Inet_addr(const std::string &host)
{
    char *tmp = new char[host.size() + 1];
    char *head = tmp;
    for (auto t : host)
        *(head++) = t;

    in_addr_t result = inet_addr(tmp);
    delete[] tmp;
    return result;
}

const char *inet_ntop(int af, const void *src,
                      char *dst, socklen_t size);

std::string Inet_ntop(int af, const void *src,
                      char *dst, socklen_t size)
{
    std::string result;
    const char *tmp = inet_ntop(af, src, dst, size);

    while (*tmp != '\0')
    {
        result += *tmp;
        tmp++;
    }
    return result;
}
