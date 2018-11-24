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
#include <tiny_base/api.h>
#include <tiny_base/log.h>

#include <iostream>
#include <string.h>
#include <map>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/epoll.h>
#include <signal.h>
#include <netinet/tcp.h>
#include <sys/syscall.h>
#include <sys/types.h>

pid_t gettid()
{
    return (pid_t)(syscall(SYS_gettid));
}

void handle_error(const char *msg)
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(-1);
}

void handle_error_s(const std::string &msg)
{
    const char *ms = new char[msg.size() + 1];
    ms = msg.c_str();
    handle_error(ms);
}

void setnonblocking(int sock)
{
    int opts;
    opts = fcntl(sock, F_GETFL);
    if (opts < 0)
    {
        char msg[] = "fcntl(sock, GETFL)";
        handle_error(msg);
    }
    opts = opts | O_NONBLOCK;
    if (fcntl(sock, F_SETFL, opts) < 0)
    {
        char msg[] = "fcntl(sock,SETFL,opts)";
        handle_error(msg);
    }
}

void getLines(const std::string &s, std::vector<std::string> &res, std::string &resOpenLine)
{
}

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

void splitString(const std::string &s, const std::string &p,
        std::vector<std::string> &result)
{
    std::string::size_type begin, end;
    begin = 0;
    end = s.find(p, begin);

    while (std::string::npos != end)
    {
        result.push_back(s.substr(begin, end - begin));

        begin = end + p.size();
        end = s.find(p, begin);
    }

    if (begin != s.size())
        result.push_back(s.substr(begin));
}

void eraseSpace(std::string &s)
{
    if (!s.empty())
    {
        s.erase(0, s.find_first_not_of(" "));
        s.erase(s.find_last_not_of(" ") + 1);
    }
}

void eraseAllSpace(std::string &s)
{
    size_t index = 0;
    if (!s.empty())
    {
        while ((index = s.find(' ', index)) != std::string::npos)
        {
            s.erase(index, 1);
        }
    }
}

std::map<char, std::string> getOption(int argc, char *argv[], const struct option *longopts, const char *short_options)
{
    std::map<char, std::string> result;
    int c;
    while ((c = getopt_long(argc, argv, short_options, longopts, NULL)) != -1)
    {
        if (optarg == nullptr)
            result[c] = " ";
        else
            result[c] = cstr2string(optarg);
    }

    return result;
}

int createSocket(int domain, int type, int protocol)
{
    int res = socket(domain, type, protocol);
    if (res < 0)
        handle_error("socket error");
    return res;
}

int createSocket()
{
    int res = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (res < 0)
        handle_error("socket error");
    return res;
}

int getSocketError(int sockfd)
{
    int opt;
    socklen_t len = sizeof(opt);
    int return_val = getsockopt(sockfd,
            SOL_SOCKET, SO_ERROR,
            &opt, &len);
    if (return_val == 0)
    {
        return opt;
    }
    else
    {
        return errno;
    }
}

int createNoBlockSocket(int domain, int type, int protocol)
{
    int res = createSocket(domain, type, protocol);
    setNoBlock(res);
    setCLOEXEC(res);
    return res;
}

int createNoBlockSocket()
{
    int res = createSocket();
    setNoBlock(res);
    setCLOEXEC(res);
    return res;
}

void Close(int fd)
{
    int res = close(fd);
    if (res < 0)
        handle_error("close error");
}

int Bind(int sockfd, const struct sockaddr_in *addr, socklen_t addrlen)
{
    int res = bind(sockfd, (const struct sockaddr *)addr, addrlen);
    if (res < 0)
        handle_error("bind error");
    return res;
}

int Listen(int sockfd, int backlog)
{
    int res = listen(sockfd, backlog);
    if (res < 0)
        handle_error("listen error");
    return res;
}

int Accept(int sockfd, struct sockaddr_in *peraddr)
{
    socklen_t len = sizeof(struct sockaddr_in);
    int res = accept(sockfd, (struct sockaddr *)peraddr, &len);
    if (res < 0)
        handle_error("accept error");
    return res;
}

int Connect(int sockfd, struct sockaddr_in *peraddr)
{
    socklen_t len = sizeof(struct sockaddr_in);
    int res = connect(sockfd, (struct sockaddr *)peraddr, len);
    return res;
}

void ShutdownWrite(int sockfd)
{
    if (shutdown(sockfd, SHUT_WR) < 0)
        handle_error("shutdownWrite error");
}

int Open(const char *pathname, int flags, mode_t mode)
{

    int res = open(pathname, flags, mode);
    if (res < 0)
    {
        char msg[] = "open error";
        handle_error(msg);
    }
    return res;
}

int Stat(const char *pathname, struct stat *buf)
{

    int res = stat(pathname, buf);
    if (res < 0)
    {
        char msg[] = "stat error";
        handle_error(msg);
    }
    return res;
}

void *Mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset)
{
    void *ptr;

    if ((ptr = mmap(addr, len, prot, flags, fd, offset)) == ((void *)-1))
    {
        char msg[] = "mmap error";
        handle_error(msg);
    }
    return (ptr);
}

int Munmap(void *start, size_t length)
{

    int res = munmap(start, length);
    if (res < 0)
    {
        char msg[] = "munmap error";
        handle_error(msg);
    }
    return res;
}

ssize_t writeString(int sockfd, const std::string &str)
{
    char *tmp = new char[str.size() + 1];
    char *head = tmp;
    for (auto t : str)
        *(head++) = t;
    *head = '\0';

    ssize_t result = Rio_writen(sockfd, tmp, strlen(tmp));
    delete[] tmp;
    return result;
}

ssize_t Rio_writen(int fd, void *usrbuf, size_t n)
{
    size_t nleft = n;
    ssize_t nwritten;
    char *bufp = (char *)usrbuf;

    while (nleft > 0)
    {
        if ((nwritten = write(fd, bufp, nleft)) <= 0)
        {
            if (errno == EINTR) /* Interrupted by sig handler return */
                nwritten = 0;   /* and call write() again */
            else
                return -1; /* errno set by write() */
        }
        nleft -= nwritten;
        bufp += nwritten;
    }
    return n;
}
ssize_t writeHtml(int sockfd, const std::string &f)
{
    size_t result;
    char *filename = new char[f.size() + 1];
    char *head = filename;
    for (auto t : f)
        *(head++) = t;
    *(head++) = '\0'; // very very important

    struct stat sbuf;
    Stat(filename, &sbuf);
    size_t filesize = sbuf.st_size;
    int filefd = Open(filename, O_RDONLY, 0);
    char *srcp = (char *)Mmap(0, filesize, PROT_READ, MAP_PRIVATE, filefd, 0); //line:netp:servestatic:mmap
    Close(filefd);                                                             //line:netp:servestatic:close
    result = Rio_writen(sockfd, srcp, filesize);                               //line:netp:servestatic:write
    Munmap(srcp, filesize);

    delete[] filename;
    return result;
}

int get_line(int sock, char *buf, int size)
{
    int i = 0;
    char c = '\0';
    int n;

    while ((i < size - 1) && (c != '\n'))
    {
        n = recv(sock, &c, 1, 0);
        /* DEBUG printf("%02X\n", c); */
        if (n > 0)
        {
            if (c == '\r')
            {
                n = recv(sock, &c, 1, MSG_PEEK);
                /* DEBUG printf("%02X\n", c); */
                if ((n > 0) && (c == '\n'))
                    recv(sock, &c, 1, 0);
                else
                    c = '\n';
            }
            buf[i] = c;
            i++;
        }
        else
            c = '\n';
    }
    buf[i] = '\0';

    return (i);
}

in_addr_t Inet_addr(const std::string &host)
{
    char *tmp = new char[host.size() + 1];
    char *head = tmp;
    for (auto t : host)
        *(head++) = t;
    *(head++) = '\0'; // very very important

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

void IpPortToSockAddr(const char *ip, int port, struct sockaddr_in *res)
{
    res->sin_family = AF_INET;
    res->sin_port = hostToNet16(port);
    inet_pton(AF_INET, ip, &(res->sin_addr));
}

void SockAddrToIpPort(char *ip, int size, int &port, const struct sockaddr_in *src)
{
    //before we use this function,we have to promise the length of ip
    //have INET_ADDRSETRLEN

    inet_ntop(AF_INET, &(src->sin_addr), ip, size); //convert to ip
    port = netToHost16(src->sin_port);
}

int setNoBlock(int fd)
{
    int status = fcntl(fd, F_GETFD);
    int newstatus = status | O_NONBLOCK;
    int res = fcntl(fd, F_SETFD, newstatus);
    if (-1 == res)
    {
        LOG(Debug) << "setNoBlock error" << std::endl;
    }

    return status;
}

int setCLOEXEC(int fd)
{
    int status = fcntl(fd, F_GETFD);
    int newstatus = status | FD_CLOEXEC;
    int res = fcntl(fd, F_SETFD, newstatus);
    if (-1 == res)
    {
        LOG(Debug) << "setCLOEXEC error" << std::endl;
    }
    return status;
}

int setSocketReuseAddress(int sockfd_)
{
    int optval = 1;
    int res = setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR,
            &optval, sizeof optval);
    if (-1 == res)
    {
        LOG(Debug) << "setSocketReuseAddress error" << std::endl;
    }
    return 0;
}

void setTcpDelay(int fd, bool on)
{
    //if on == false:
    //      tcp no delay.
    //else:
    //      tcp delay:enable Nagle algorithm.
    int optval = on ? 0 : 1;
    int res = setsockopt(fd, IPPROTO_TCP, TCP_NODELAY,
            &optval, sizeof(optval));
    if (res != 0)
        handle_error_s("setsockopt error\n");
}

void enableTcpDelay(int fd)
{
    setTcpDelay(fd, true);
}

void disableTcpDelay(int fd)
{
    setTcpDelay(fd, false);
}

void setTcpKeepAlive(int fd, bool on)
{
    //if on == false:
    //      tcp no keep alive.
    //else:
    //      enable tcp keep alive.
    int optval = on ? 1 : 0;
    int res = setsockopt(fd, IPPROTO_TCP, SO_KEEPALIVE,
            &optval, sizeof(optval));
    if (res != 0)
        handle_error_s("setsockopt error\n");
}

void enableTcpKeepAlive(int fd)
{
    setTcpKeepAlive(fd, true);
}

void disableTcpKeepAlive(int fd)
{
    setTcpKeepAlive(fd, false);
}

struct sockaddr_in getLocalAddr(int sockfd)
{
    struct sockaddr_in localaddr;
    bzero(&localaddr, sizeof localaddr);
    socklen_t addrlen = sizeof(localaddr);
    if (getsockname(sockfd, (struct sockaddr *)(&localaddr), &addrlen) < 0)
        handle_error("getsockname error");
    return localaddr;
}

void epolladdfd(int epfd, int fd, int events)
{
    struct epoll_event event;
    bzero(&event, sizeof(event));
    event.data.fd = fd;
    event.events = events;
    int res = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event);
    if (res == -1)
    {
        char msg[] = "epoll add fd error";
        handle_error(msg);
    }
    setnonblocking(fd);
}

void epoll_modfd(int epfd, int fd, int events)
{
}

void epollremovefd(int epfd, int fd)
{
    int res = epoll_ctl(epfd, EPOLL_CTL_DEL, fd, 0);
    if (res == -1)
    {
        char msg[] = "epoll remove fd error";
        handle_error(msg);
    }
}

void add_signal(int sign, sighandler_t handler)
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handler;
    sa.sa_flags |= SA_RESTART;
    sigfillset(&sa.sa_mask);
    int res = sigaction(sign, &sa, nullptr);
    if (res == -1)
    {
        char msg[] = "add signal error";
        handle_error(msg);
    }
}

void remove_signal(int sign)
{
    //FIXME:
}
